# Historial de cambios

**Español** · Los cambios se apuntan aquí por versión del firmware. Las piezas 3D llevan su propia numeración (carcasa v2.x).
**English** · Changes are listed by firmware version. The 3D parts have their own numbering (case v2.x).

El formato sigue [Keep a Changelog](https://keepachangelog.com/es-ES/1.1.0/) y las versiones siguen [SemVer](https://semver.org/lang/es/).

## [1.5.0] — 2026-11-02 · Versión para producción

### Añadido
- **Menú de pruebas (avanzado)** en el portal, cerrado por defecto: estado del sistema, diagnóstico de pantalla y pruebas del zumbador.
- Interruptor **«Tengo un zumbador conectado»** en los ajustes. Si está apagado, nada del sonido aparece en el portal ni en la página de mensajes.

### Cambiado
- El sonido pasa a ser un **añadido opcional (en desarrollo)**: viene apagado de fábrica.
- Revisión completa del código y de los README en español e inglés.

### Arreglado
- Tabla de conexiones partida en los README.

## [1.4.8] — 2026-10-26
### Arreglado
- La marca en la memoria RTC se guarda fuera de la zona que usa el arranque para las actualizaciones (bloque 64).
### Añadido
- Pitido de prueba del zumbador a 2,5 kHz.

## [1.4.7] — 2026-10-26
### Arreglado
- **«No conecta» al buscar versión nueva**: faltaba memoria para HTTPS. Se usa la opción de memoria `mmu=4816H` (segundo montón en IRAM).

## [1.4.6] — 2026-10-26
### Arreglado
- El aviso de «colgado» solo sale si hubo un fallo real (excepción o perro guardián).

## [1.4.5] — 2026-10-26
### Arreglado
- Tras actualizarse muestra «Actualizado» en lugar de «colgado».

## [1.4.4] — 2026-10-26
### Arreglado
- Evita bucles de reinicio al buscar actualizaciones.

## [1.4.3] — 2026-10-26
### Cambiado
- Mensajes claros (con el motivo del error y la memoria libre) al buscar una versión nueva.

## [1.4.2] — 2026-10-26
### Arreglado
- Deshace el cambio de pantalla de la 1.4.1 (la configuración vuelve a reenviarse cada 2 s).
### Cambiado
- Brillo de fábrica más alto.

## [1.4.1] — 2026-10-26
### Cambiado
- Intento de estabilizar la pantalla al desplazar texto (retirado en la 1.4.2).

## [1.4.0] — 2026-10-25
### Añadido
- **Modo concentración**: solo la hora, sin parpadeos, fecha ni aventuras. Manual o por horario.
- **Zumbador opcional**: melodías originales, volumen regulable, avisos con los mensajes y **alarma de mañana**.

## [1.3.0] — 2026-10-25
### Añadido
- **Página de mensajes para el móvil** (`/mensaje`): personaje de entrada, duración, velocidad, vista previa e icono para la pantalla de inicio de iOS y Android.

## [1.2.4] — 2026-10-25
### Arreglado
- El botón de enviar un mensaje no respondía.

## [1.2.3] — 2026-10-25
### Añadido
- La pantalla se recupera sola si se desconfigura.

## [1.2.2] — 2026-10-25
### Añadido
- Asistente de orientación de la pantalla.

## [1.2.1] — 2026-10-25
### Arreglado
- Pantalla apagada en todas las placas: el pin CS (D6) se configura después de iniciar SPI.

## [1.2.0] — 2026-10-24
### Añadido
- Diagnóstico de pantalla: prueba de LEDs y cambio de pines.

## [1.1.1] — 2026-10-19
### Arreglado
- Recupera el portal en español e inglés.

## [1.0.0] — 2026-10-18
- Primera versión pública.

---

## Carcasa 3D

- **2026-11-02** · Aro del zumbador holgado (12,8 mm, nervios que solo rozan) en `soporte_wemos_zumbador.stl` y `tapa_trasera_zumbador.stl`: el aro anterior estrujaba el zumbador y casi no se oía.
- **2026-10-26** · `soporte_wemos_zumbador.stl`: Wemos con pines soldados y aro que sujeta el propio zumbador.
- **2026-10-25** · Dos tapas traseras (con y sin zumbador). Cabe la matriz de 130 mm y la ventana difusora se puede colocar.
- **2026-10-24** · Logo en una sola pieza para imprimir en dos colores con pausa.
- **2026-10-19** · **v2.4**: tornillos M3×6 avellanados autorroscantes, sin insertos, todos desde atrás. Soporte atornillado para el Wemos.
- **2026-10-18** · **v2**: diseño de bloques de piedra.

[1.5.0]: https://github.com/wencescarlos/reloj-hyrule/releases/tag/v1.5.0
