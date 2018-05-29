/**
 * @brief Interfaz de funciones personales acercas de semaforos.
 *
 * Este fichero contiene las funciones personales acerca de semaforos.
 *
 * @file semaforos.h
 * @author Profesores de SOPER (EDIT: Rafael Sánchez & Sergio Galán).
 * @version 1.0
 * @date 06-04-2018
 */
#ifndef SEMAFOROS
#define SEMAFOROS

#define OK EXIT_SUCCESS /*!< Todo fue bien */
#define ERROR -1 /*!< Error */

/**
 * @brief Inicializa los semaforos indicados.
 *
 * @param semid Identificador del semaforo.
 * @param array Valores iniciales.
 * @return OK si todo fue correcto, ERROR en caso de error.
 */
int inicializar_semaforo(int semid, unsigned short *array);

/**
 * @brief Borra un semáforo
 *
 * @param semid Identificador del semaforo.
 * @return OK si todo fue correcto, ERROR en caso de error.
 */
int borrar_semaforo(int semid);

/**
 * @brief Crea un semaforo con la clave y el tamaño especificado. Lo inicializa a 0.
 *
 * @param key Clave precompartida del semaforo.
 * @param size Tamaño del semaforo.
 * @param semid Identificador del semaforo.
 * @return ERROR en caso de error, 0 si ha creado el semaforo, 1 si ya estaba creado.
 */
int crear_semaforo(key_t key, int size, int *semid);

/**
 * @brief Baja el semaforo indicado
 *
 * @param semid Identificador del semaforo.
 * @param num_sem Semaforo dentro del array.
 * @param undo Flag de modo persistente pese a finalización abrupta.
 * @return OK si todo fue correcto, ERROR en caso de error.
 */
int down_semaforo(int semid, int num_sem, int undo);

/**
 * @brief Baja todos los semaforos del array indicado por active.
 *
 * @param semid: Identificador del semaforo.
 * @param size: Tamaño de active.
 * @param undo: Flag de modo persistente pese a finalización abrupta.
 * @param active: Semaforos involucrados. Array de enteros.
 * @return int: OK si todo fue correcto, ERROR en caso de error.
 */
int down_multiple_semaforo(int semid, int size, int undo, int *active);

/**
 * @brief Sube el semaforo indicado
 *
 * @param semid Identificador del semaforo.
 * @param num_sem Semaforo dentro del array.
 * @param undo Flag de modo persistente pese a finalización abrupta.
 * @return OK si todo fue correcto, ERROR en caso de error.
 */
int up_semaforo(int semid, int num_sem, int undo);

/**
 * @brief Sube todos los semaforos del array indicado por active.
 *
 * @param semid: Identificador del semaforo.
 * @param size: Tamaño de active.
 * @param undo: Flag de modo persistente pese a finalización abrupta.
 * @param active: Semaforos involucrados. Array de enteros.
 * @return int: OK si todo fue correcto, ERROR en caso de error.
 */
int up_multiple_semaforo(int semid, int size, int undo, int *active);

#endif
