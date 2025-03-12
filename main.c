/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 22:34:17 by jgamarra          #+#    #+#             */
/*   Updated: 2025/03/12 18:21:49 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct cmd *parseline(char**, char*);
struct cmd *parsepipe(char**, char*);
struct cmd *parseexec(char**, char*);
struct cmd *nulterminate(struct cmd*);
struct cmd* parseheredoc(char **ps, char *es, struct cmd *subcmd);

char whitespace[] = " \t\r\n\v";
char symbols[] = "<|>&;()";

void	print_vector(char **vector)
{
	int	i;

	i = 0;
	while (vector[i])
	{
		printf("vector[%d]: %s\n", i, vector[i]);
		i++;
	}
}

// NUL-terminate all the counted strings.
struct cmd*
nulterminate(struct cmd *cmd)
{
  int i;
  // struct backcmd *bcmd;
  struct execcmd *ecmd;
  // struct listcmd *lcmd;
  struct pipecmd *pcmd;
  struct redircmd *rcmd;

  if(cmd == 0)
    return 0;

  switch(cmd->type){
	case HDOC:
		break ;
		
  case EXEC:
    ecmd = (struct execcmd*)cmd;
    for(i=0; ecmd->argv[i]; i++)
      *ecmd->eargv[i] = 0;
    break;

  case REDIR:
    rcmd = (struct redircmd*)cmd;
    nulterminate(rcmd->cmd);
    *rcmd->efile = 0;
    break;

  case PIPE:
    pcmd = (struct pipecmd*)cmd;
    nulterminate(pcmd->left);
    nulterminate(pcmd->right);
    break;
  }
  return cmd;
}

static void
ft_putc(int fd, char c)
{
  write(fd, &c, 1);
}

static void
printint(int fd, int xx, int base, int sgn)
{
  static char digits[] = "0123456789ABCDEF";
  char buf[16];
  int i, neg;
  uint x;

  neg = 0;
  if(sgn && xx < 0){
    neg = 1;
    x = -xx;
  } else {
    x = xx;
  }

  i = 0;
  do{
    buf[i++] = digits[x % base];
  }while((x /= base) != 0);
  if(neg)
    buf[i++] = '-';

  while(--i >= 0)
    ft_putc(fd, buf[i]);
}

// Print to the given fd. Only understands %d, %x, %p, %s.
void
ft_printf_sh(int fd, const char *fmt, ...)
{
  char *s;
  int c, i, state;
  uint *ap;

  state = 0;
  ap = (uint*)(void*)&fmt + 1;
  for(i = 0; fmt[i]; i++){
    c = fmt[i] & 0xff;
    if(state == 0){
      if(c == '%'){
        state = '%';
      } else {
        ft_putc(fd, c);
      }
    } else if(state == '%'){
      if(c == 'd'){
        printint(fd, *ap, 10, 1);
        ap++;
      } else if(c == 'x' || c == 'p'){
        printint(fd, *ap, 16, 0);
        ap++;
      } else if(c == 's'){
        s = (char*)*ap;
        ap++;
        if(s == 0)
          s = "(null)";
        while(*s != 0){
          ft_putc(fd, *s);
          s++;
        }
      } else if(c == 'c'){
        ft_putc(fd, *ap);
        ap++;
      } else if(c == '%'){
        ft_putc(fd, c);
      } else {
        // Unknown % sequence.  Print it to draw attention.
        ft_putc(fd, '%');
        ft_putc(fd, c);
      }
      state = 0;
    }
  }
}

int
gettoken(char **ps, char *es, char **q, char **eq)
{
  char *s;
  int ret;

  s = *ps;
  while(s < es && strchr(whitespace, *s))
    s++;
  if(q)
    *q = s;
  ret = *s;
  switch(*s){
  case 0:
    break;
  case '|':
//   case '(':
//   case ')':
//   case ';':
//   case '&':
  case '<':
    s++;
    break;
  case '>':
    s++;
    if(*s == '>'){
      ret = '+';
      s++;
    }
    break;
  default:
    ret = 'a';
    while(s < es && !strchr(whitespace, *s) && !strchr(symbols, *s))
      s++;
    break;
  }
  if(eq)
    *eq = s;

  while(s < es && strchr(whitespace, *s))
    s++;
  *ps = s;
  return ret;
}

void
panic(char *s)
{
  ft_printf_sh(2, "%s\n", s);
  exit(1);
}

int
fork1(void)
{
  int pid;

  pid = fork();
  if(pid == -1)
    panic("fork");
  return pid;
}

int
peek(char **ps, char *es, char *toks)
{
  char *s;

  s = *ps;
  while(s < es && strchr(" \t\r\n\v", *s))
    s++;
  *ps = s;
  return *s && strchr(toks, *s);
}

struct cmd*
pipecmd(struct cmd *left, struct cmd *right)
{
  struct pipecmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = PIPE;
  cmd->left = left;
  cmd->right = right;
  return (struct cmd*)cmd;
}

struct cmd*
execcmd(void)
{
  struct execcmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = EXEC;
  return (struct cmd*)cmd;
}

struct cmd*
redircmd(struct cmd *subcmd, char *file, char *efile, int mode, int fd)
{
  struct redircmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = REDIR;
  cmd->cmd = subcmd;
  cmd->file = file;
  cmd->efile = efile;
  cmd->mode = mode;
  cmd->fd = fd;
  return (struct cmd*)cmd;
}

struct cmd* parseredirs(struct cmd *cmd, char **ps, char *es) {
    int tok;
    char *q, *eq;

    while (peek(ps, es, "<>")) {
        // Lee el token del operador de redirección sin avanzar los argumentos.
        tok = gettoken(ps, es, 0, 0);

        // Si se detecta un heredoc: token '<' y el siguiente carácter es también '<'
        if (tok == '<' && **ps == '<') {
            (*ps)++;  // Consume el segundo '<'
            // Procesa el heredoc: consume el delimitador y construye el nodo HDOC.
            cmd = parseheredoc(ps, es, cmd);
        }
        else {
            // Procesa redirecciones simples (<, > o >>)
            if (gettoken(ps, es, &q, &eq) != 'a')
                panic("Falta el argumento para la redirección");
            switch (tok) {
                case '<':
                    cmd = redircmd(cmd, q, eq, O_RDONLY, 0);
                    break;
                case '>':
                    cmd = redircmd(cmd, q, eq, O_WRONLY | O_CREAT | O_TRUNC, 1);
                    break;
                case '+':  // para >>
                    cmd = redircmd(cmd, q, eq, O_WRONLY | O_CREAT | O_APPEND, 1);
                    break;
                default:
                    panic("Operador de redirección desconocido");
            }
        }
    }
    return cmd;
}

struct cmd*
parseexec(char **ps, char *es)
{
  char *q, *eq;
  int tok, argc;
  struct execcmd *cmd;
  struct cmd *ret;

//   if(peek(ps, es, "("))
//     return parseblock(ps, es);
  // printf("ps: %s\n", *ps);
  ret = execcmd();
  cmd = (struct execcmd*)ret;

  argc = 0;
  ret = parseredirs(ret, ps, es);
  while(!peek(ps, es, "|)&;")){ // loop character by character
    if((tok=gettoken(ps, es, &q, &eq)) == 0)
      break;
    if(tok != 'a')
      panic("syntax");
    cmd->argv[argc] = q;
    cmd->eargv[argc] = eq;
    argc++;
    if(argc >= MAXARGS)
      panic("too many args");
    ret = parseredirs(ret, ps, es);
  }
  // print_vector(cmd->argv);
  // print_vector(cmd->eargv);
  cmd->argv[argc] = 0;
  cmd->eargv[argc] = 0;
  return ret;
}

struct cmd*
parsepipe(char **ps, char *es)
{
  struct cmd *cmd;

  // printf("parsepipe->ps: %s\n", *ps);
  cmd = parseexec(ps, es);
  if(peek(ps, es, "|")){
    gettoken(ps, es, 0, 0);
    cmd = pipecmd(cmd, parsepipe(ps, es));
  }
  return cmd;
}

struct cmd* heredoccmd(struct cmd *subcmd, char *delim, char *edelim)
{
    struct heredoccmd *cmd = malloc(sizeof(*cmd));
    if (!cmd)
        panic("malloc heredoccmd failed");
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = HDOC;
    cmd->cmd = subcmd;
    cmd->delim = delim;
    cmd->edelim = edelim;  // Este campo es opcional según tu implementación.
    return (struct cmd*)cmd;
}

struct cmd*
parseline(char **ps, char *es)
{
  struct cmd *cmd;

  cmd = parsepipe(ps, es);
//   while(peek(ps, es, "&")){
//     gettoken(ps, es, 0, 0);
//     cmd = backcmd(cmd);
//   }
//   if(peek(ps, es, ";")){
//     gettoken(ps, es, 0, 0);
//     cmd = listcmd(cmd, parseline(ps, es));
//   }
  return cmd;
}

struct cmd*
parsecmd(char *s)
{
  char *es;
  struct cmd *cmd;

  es = s + strlen(s); // es = end of string
  cmd = parseline(&s, es); // s = start of all the string
  peek(&s, es, "");
  if(s != es){
    ft_printf_sh(2, "leftovers: %s\n", s);
    panic("syntax");
  }
  nulterminate(cmd);
  return cmd;
}
struct cmd* parseheredoc(char **ps, char *es, struct cmd *subcmd)
{
    char *q, *eq;
    
    // Se espera un token de tipo 'a' que representa el delimitador.
    if (gettoken(ps, es, &q, &eq) != 'a')
        panic("Falta el delimitador para heredoc");
    
    // Crea el nodo heredoccmd con el subcomando ya parseado y
    // el delimitador (q para delim, eq para edelim) que se acaba de leer.
    return heredoccmd(subcmd, q, eq);
}

void runcmd(struct cmd *cmd) {
    int p[2];
    struct execcmd *ecmd;
    struct pipecmd *pcmd;
    struct redircmd *rcmd;
    struct heredoccmd *hcmd;

    if (cmd == 0)
        exit(0);

    switch (cmd->type) {
        case EXEC:
		ecmd = (struct execcmd *)cmd;
		if (ecmd->argv[0] == 0) {
			fprintf(stderr, "Error: comando vacío\n");
			exit(0);
		}
		fprintf(stderr, "Ejecutando: '%s'\n", ecmd->argv[0]);
		execvp(ecmd->argv[0], ecmd->argv);
		perror("exec failed");
		exit(1);

        case REDIR:
            rcmd = (struct redircmd *)cmd;
            {
                int fd = open(rcmd->file, rcmd->mode, 0644);
                if (fd < 0) {
                    perror("open failed");
                    exit(1);
                }
                if (dup2(fd, rcmd->fd) < 0) {
                    perror("dup2 failed");
                    exit(1);
                }
                close(fd);
            }
            runcmd(rcmd->cmd);
            break;

			case HDOC:
			{
				// Convertir el nodo genérico a heredoccmd
				struct heredoccmd *hcmd = (struct heredoccmd *)cmd;
				if (hcmd == NULL || hcmd->cmd == NULL || hcmd->delim == NULL) {
					fprintf(stderr, "Error: Comando heredoc o delimitador inválido.\n");
					exit(EXIT_FAILURE);
				}
				
				// Crear un pipe para almacenar la entrada del heredoc.
				int pipefd[2];
				if (pipe(pipefd) < 0) {
					perror("pipe failed");
					exit(EXIT_FAILURE);
				}
				
				// Se forkea: el hijo se encargará de leer desde stdin (con getline)
				// hasta encontrar el delimitador y escri./mibir lo leído en el pipe.
				if (fork1() == 0) {
					// Proceso hijo: escribe en el pipe.
					close(pipefd[0]);  // Cierra el extremo de lectura.
					
					char *line = NULL;
					size_t len = 0;
					while (1) {
						// Muestra un prompt propio para el heredoc.
						write(STDOUT_FILENO, "> ", 2);
						
						ssize_t nread = getline(&line, &len, stdin);
						if (nread < 0) {
							perror("getline failed");
							free(line);
							exit(EXIT_FAILURE);
						}
						
						// Elimina el salto de línea para comparar correctamente.
						line[strcspn(line, "\n")] = '\0';
						
						// Si se alcanza el delimitador, se interrumpe el bucle.
						if (strcmp(line, hcmd->delim) == 0)
							break;
						fprintf(stderr, "Escribiendo en heredoc: %s\n", line);

						
						// Escribe la línea en el pipe, agregando el salto de línea.
						if (write(pipefd[1], line, strlen(line)) < 0 ||
							write(pipefd[1], "\n", 1) < 0) {
							perror("write failed");
							free(line);
							exit(EXIT_FAILURE);
						}
					}
					free(line);
					close(pipefd[1]);  // Cierra el extremo de escritura.
					exit(EXIT_SUCCESS); // El proceso hijo termina.
				}
				
				// Proceso padre:
				close(pipefd[1]);  // Cierra el extremo de escritura.
				// Redirige la entrada estándar para que provenga del pipe.
				if (dup2(pipefd[0], STDIN_FILENO) < 0) {
					perror("dup2 failed");
					exit(EXIT_FAILURE);
				}
				close(pipefd[0]);  // Se cierra el descriptor duplicado.
				
				// Espera a que el hijo finalice la escritura del heredoc.
				wait(NULL);
				
				// Ejecuta el subcomando. Ese comando leerá su entrada estándar (ya redirigida)
				// y así procesará el contenido del heredoc.
				runcmd(hcmd->cmd);
				
				break;
			}
			
        case PIPE:
            pcmd = (struct pipecmd *)cmd;
            if (pipe(p) < 0)
                panic("pipe");
            if (fork1() == 0) {
                close(1);
                dup(p[1]);
                close(p[0]);
                close(p[1]);
                runcmd(pcmd->left);
            }
            if (fork1() == 0) {
                close(0);
                dup(p[0]);
                close(p[0]);
                close(p[1]);
                runcmd(pcmd->right);
            }
            close(p[0]);
            close(p[1]);
            wait(0);
            wait(0);
            break;
    }
    exit(0);
}


int main(int argc, char **argv, char **envp)
{
    t_minishell minishell;
    char *input;

    valid_inital_param(argc, envp, &minishell);

    /* Asegúrate de que la variable PATH esté definida.
       Si no lo está, establécela para que execvp pueda encontrar los ejecutables. */
	   if (getenv("PATH") == NULL) {
        setenv("PATH", "/bin:/usr/bin", 1);
	   }
	   printf("PATH: %s\n", getenv("PATH"));

    catch_signal();
    while (1)
    {
        if (isatty(fileno(stdin)))
            input = readline(PROMPT);
        else
        {
            char *line;
            line = get_next_line(fileno(stdin));
            if (!line)
                break;
            input = ft_strtrim(line, "\n");
            free(line);
        }
        catch_interactive(input, PROMPT);
        input = check_input_valid(input);
        if (input[0] == '\0')
        {
            free(input);
            continue;
        }
        if(fork1() == 0)
            runcmd(parsecmd(input));
        wait(0);
    }
    safe_free_minishell(&minishell);
    return 0;
}
