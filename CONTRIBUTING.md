# Cómo colaborar · Contributing

**Español** · ¡Gracias por querer mejorar el Reloj de Hyrule! Es un proyecto de fans **sin ánimo de lucro** ([CC BY-NC-SA 4.0](LICENSE)): todo lo que aportes se comparte con la misma licencia.

- **¿Algo falla?** Abre una [incidencia](https://github.com/wencescarlos/reloj-hyrule/issues/new/choose) con la versión del firmware y el «Estado del sistema» del menú de pruebas.
- **¿Un cambio?** Haz un *fork*, crea una rama y abre un *pull request* contra `main`.
- **Firmware:** sube `VERSION_RELOJ` en `firmware/RelojHyrule/configuracion.h` y apunta el cambio en [CHANGELOG.md](CHANGELOG.md). Al unirse a `main`, GitHub Actions lo compila y los relojes se actualizan solos. Para publicar además una *release* con el `.bin` y las piezas: pestaña *Actions* → **Crear release** → *Run workflow*.
- **Textos del portal:** cada texto en español necesita su traducción en el diccionario `EN` de la página.
- **Piezas 3D:** se generan con `modelos_3d/generar_modelos.py`; cambia el script, no los STL a mano.
- **Nada de material de Nintendo:** ni música, ni sprites, ni logos oficiales. Todo debe ser original (ver [AVISO_LEGAL.md](AVISO_LEGAL.md)).

**English** · Thanks for helping! This is a **non-profit** fan project (CC BY-NC-SA 4.0). Report bugs through the issue templates, send pull requests against `main`, bump `VERSION_RELOJ` and update the changelog for firmware changes, add an English entry for every new portal text, change 3D parts through the generator script, and never add Nintendo material (music, sprites, logos).
