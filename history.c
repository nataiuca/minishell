/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 18:18:56 by natferna          #+#    #+#             */
/*   Updated: 2025/03/15 00:00:00 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Crea e inicializa una nueva lista de historial */
t_history *history_create(void)
{
    t_history *hist = malloc(sizeof(t_history));
    if (!hist)
        return NULL;
    hist->capacity = 16;
    hist->count = 0;
    hist->entries = malloc(sizeof(char *) * hist->capacity);
    if (!hist->entries)
    {
        free(hist);
        return NULL;
    }
    return hist;
}

/* Agrega una entrada al historial propio y llama a add_history */
void history_add(t_history *hist, const char *entry)
{
    size_t i;
    size_t len;

    if (!hist || !entry)
        return;
    if (hist->count >= hist->capacity)
    {
        int new_capacity = hist->capacity * 2;
        char **new_entries = malloc(sizeof(char *) * new_capacity);
        if (!new_entries)
            return;
        /* Copiar los punteros existentes */
        i = 0;
        while (i < (size_t)hist->count)
        {
            new_entries[i] = hist->entries[i];
            i++;
        }
        free(hist->entries);
        hist->entries = new_entries;
        hist->capacity = new_capacity;
    }
    /* Calcula la longitud de la cadena */
    len = 0;
    while (entry[len])
        len++;
    /* Copia la cadena en memoria nueva */
    char *copy = malloc(len + 1);
    if (!copy)
        return;
    i = 0;
    while (i < len)
    {
        copy[i] = entry[i];
        i++;
    }
    copy[i] = '\0';
    hist->entries[hist->count] = copy;
    hist->count++;
}

/*
 * Construye la ruta al archivo de historial *
 * El resultado es: "<HOME>/<HISTFILE_NAME>"
 */
char *construct_history_path(const char *histfile_name)
{
    char *home = getenv("HOME");
    if (!home)
    {
        printf("Error: La variable de entorno HOME no está definida.\n");
        return NULL;
    }
    size_t home_len = 0;
    while (home[home_len])
        home_len++;
    size_t filename_len = 0;
    while (histfile_name[filename_len])
        filename_len++;
    
    /* Reserva espacio para: home + '/' + filename + '\0' */
    size_t total = home_len + 1 + filename_len + 1;
    char *path = malloc(total);
    if (!path)
    {
        printf("Error: No se pudo reservar memoria para la ruta del historial.\n");
        return NULL;
    }
    
    size_t pos = 0;
    for (size_t i = 0; i < home_len; i++)
    {
        path[pos++] = home[i];
    }
    path[pos++] = '/';
    for (size_t i = 0; i < filename_len; i++)
    {
        path[pos++] = histfile_name[i];
    }
    path[pos] = '\0';
    
    return path;
}

/*
 * Carga el historial previo desde el archivo.
 * Se leen todos los comandos (separados por '\n') y, para cada uno, se llama a
 * add_history y se almacena en la estructura t_history.
 */
void load_history_file(t_history *hist, const char *histfile_name) {
    char *path = construct_history_path(histfile_name);
    if (!path)
        return;

    if (access(path, F_OK) != 0) {  // El archivo no existe
        printf("Historial vacío o no encontrado en: %s\n", path);
        free(path);
        return;
    }

    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("Error al abrir el archivo de historial");
        free(path);
        return;
    }

    const size_t file_buffer_size = 4096;
    char *file_content = malloc(file_buffer_size);
    if (!file_content) {
        printf("Error: no se pudo reservar memoria para leer el archivo de historial.\n");
        close(fd);
        free(path);
        return;
    }

    ssize_t bytes_read = read(fd, file_content, file_buffer_size - 1);
    if (bytes_read > 0) {
        file_content[bytes_read] = '\0';
        char *start = file_content;
        for (ssize_t i = 0; i < bytes_read; i++) {
            if (file_content[i] == '\n') {
                file_content[i] = '\0';
                if (*start) {
                    add_history(start);
                    history_add(hist, start);
                }
                start = file_content + i + 1;
            }
        }
        if (*start) {
            add_history(start);
            history_add(hist, start);
        }
    } else {
        printf("No se pudo leer el contenido del archivo de historial.\n");
    }

    free(file_content);
    close(fd);
    free(path);
}

/*
 * Guarda el historial actual en el archivo.
 * Se escriben, usando write, solo las últimas 'max_entries' entradas.
 */
void save_history_file(t_history *hist, const char *histfile_name, int max_entries) {
    char *path = construct_history_path(histfile_name);
    if (!path)
        return;

    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Error al abrir el archivo de historial para escribir");
        free(path);
        return;
    }

    int start = (hist->count > max_entries) ? hist->count - max_entries : 0;
    for (int i = start; i < hist->count; i++) {
        size_t len = 0;
        while (hist->entries[i][len])
            len++;
        if (write(fd, hist->entries[i], len) < 0)
            perror("Error escribiendo entrada de historial");
        if (write(fd, "\n", 1) < 0)
            perror("Error escribiendo salto de línea");
    }

    close(fd);
    free(path);
}

