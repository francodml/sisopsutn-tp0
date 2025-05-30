#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

    log_info(logger, "Hola! Soy un log");


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

    if (config==NULL) {
        log_error(logger, "No se pudo cargar el archivo de configuracion, abortando...");
        abort();
    }

    ip = config_get_string_value(config, "IP");
    puerto = config_get_string_value(config, "PUERTO");
    valor = config_get_string_value(config, "CLAVE");

	// Loggeamos el valor de config

    log_info(logger, "Obtenido valor: %s", valor);


	/* ---------------- LEER DE CONSOLA ---------------- */

	 leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje

    enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

    nuevo_logger = log_create("tp0.log","TP0 Cliente", true, LOG_LEVEL_INFO);

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

    nuevo_config = config_create("./cliente.config");

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido = NULL; // Cambio: inicializamos el pointer a nulo para evitar apuntar a basura.

    do {
        if(leido) { // Liberamos la memoria si ya hay un valor
            free(leido);
            leido = (char*)NULL;
        }
        leido = readline("> "); // Leemos entrada del usuario
        log_info(logger, "%s\n", leido); // y la imprimimos a consola
    } while (strcmp("",leido)); // Surge la condicion del dowhile, si el string no es vacio, seguimos.

    free(leido); // Ultimo free (por las dudas)
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido = NULL;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete

    do {
        if(leido) { // Liberamos la memoria si ya hay un valor
            free(leido);
            leido = (char*)NULL;
        }
        leido = readline("> "); // Leemos entrada del usuario
        agregar_a_paquete(paquete, leido, strlen(leido)+1);
    } while (strcmp("",leido)); // Surge la condicion del dowhile, si el string no es vacio, seguimos.

    free(leido); // Ultimo free (por las dudas)

    enviar_paquete(paquete, conexion);

    eliminar_paquete(paquete);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */

      log_destroy(logger);
      
      config_destroy(config);
      
      liberar_conexion(conexion);
}
