// ===========================================================================
//  pagina_web.h  -  Portal de configuración del Reloj de Hyrule
//  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
//  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
//  Una sola página (HTML + CSS + JavaScript) guardada en la memoria flash.
//  Funciona sin internet: no usa nada de fuera.
//  En español o en inglés: el idioma se elige arriba y se guarda en el reloj.
// ===========================================================================
#pragma once
#include <Arduino.h>

const char PAGINA_PRINCIPAL[] PROGMEM = R"PAGINA(<!DOCTYPE html>
<html lang="es"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Reloj de Hyrule</title>
<style>
:root{--fondo:#0f2418;--tarjeta:#173323;--borde:#c9a227;--oro:#e8c04a;--texto:#f3ecd2;--suave:#b8c4a8;--rojo:#d9534f}
*{box-sizing:border-box}
body{margin:0;background:var(--fondo) radial-gradient(circle at 50% -20%,#2a5a3c 0,transparent 60%) no-repeat;color:var(--texto);
 font:16px/1.45 Georgia,"Times New Roman",serif;padding:16px}
main{max-width:720px;margin:0 auto}
header{text-align:center;padding:8px 0 4px}
header svg{width:64px;height:56px;filter:drop-shadow(0 0 8px #e8c04a88)}
h1{margin:6px 0 0;color:var(--oro);font-size:1.7em;letter-spacing:.06em}
.reloj{font:700 2.6em/1 "Courier New",monospace;color:#ff6b4a;text-shadow:0 0 12px #ff3b1f99;margin:10px 0 2px}
.sub{color:var(--suave);font-size:.95em}
section{background:var(--tarjeta);border:2px solid var(--borde);border-radius:14px;padding:14px 16px;margin:16px 0;
 box-shadow:0 0 0 4px #0b1a11 inset}
h2{margin:0 0 10px;color:var(--oro);font-size:1.2em;display:flex;align-items:center;gap:8px}
h2:before{content:"";width:0;height:0;border-left:7px solid transparent;border-right:7px solid transparent;border-bottom:12px solid var(--oro)}
label{display:block;margin:10px 0 4px;color:var(--suave)}
input[type=text],input[type=password],input[type=number],input[type=time],select,textarea{width:100%;padding:9px 10px;border-radius:9px;
 border:1px solid #5d7a4f;background:#0d1d13;color:var(--texto);font:inherit}
input[type=range]{width:100%;accent-color:var(--oro)}
.fila{display:flex;gap:10px}.fila>*{flex:1}
.check{display:flex;align-items:center;gap:10px;margin:10px 0;color:var(--texto)}
.check input{width:22px;height:22px;accent-color:var(--oro)}
.opciones{display:flex;gap:8px;flex-wrap:wrap}
.opciones label{margin:0;flex:1;min-width:130px;border:1px solid #5d7a4f;border-radius:9px;padding:9px;cursor:pointer;color:var(--texto);text-align:center}
.opciones input{display:none}.opciones input:checked+span{color:var(--oro);font-weight:700}
.opciones label:has(input:checked){border-color:var(--oro);background:#0d1d13}
button,.boton{display:inline-block;background:linear-gradient(#e8c04a,#b38a1b);color:#1d1606;border:0;border-radius:10px;padding:10px 14px;
 font:700 1em Georgia,serif;cursor:pointer;margin:4px 4px 4px 0;text-decoration:none}
button.sec{background:#0d1d13;color:var(--oro);border:1px solid var(--borde)}
button.peligro{background:#5a1d1b;color:#ffd7d4;border:1px solid var(--rojo)}
.aventuras{display:grid;grid-template-columns:repeat(auto-fill,minmax(150px,1fr));gap:8px}
.aventuras button{margin:0;width:100%;font-size:.9em}
.nota{color:var(--suave);font-size:.85em;margin:6px 0}
#aviso{position:fixed;left:50%;bottom:20px;transform:translateX(-50%);background:var(--oro);color:#1d1606;padding:10px 18px;border-radius:30px;
 font-weight:700;opacity:0;transition:.3s;pointer-events:none}
#aviso.ver{opacity:1}
#redes div{padding:8px;border-bottom:1px solid #2c4a37;cursor:pointer}#redes div:hover{color:var(--oro)}
#portal{display:none;border-color:#ff6b4a}
.idiomas{display:inline-flex;margin-top:10px;border:1px solid var(--borde);border-radius:30px;overflow:hidden}
.idiomas button{margin:0;border-radius:0;background:#0d1d13;color:var(--suave);padding:6px 14px;font-weight:400}
.idiomas button.activo{background:linear-gradient(#e8c04a,#b38a1b);color:#1d1606;font-weight:700}
[data-l]{display:none}html[lang=es] [data-l=es],html[lang=en] [data-l=en]{display:inline}
.dias{display:flex;gap:6px;margin:10px 0}.dias label{margin:0;flex:1;text-align:center;border:1px solid #5d7a4f;border-radius:9px;padding:8px 0;cursor:pointer;color:var(--texto)}
.dias input{display:none}.dias label:has(input:checked){background:var(--oro);color:#1d1606;font-weight:700;border-color:var(--oro)}
.botonesConc button{margin:3px 3px 3px 0}
footer{text-align:center;color:var(--suave);font-size:.8em;margin:24px 0 40px}
</style></head><body><main>

<header>
 <svg viewBox="0 0 64 56" aria-hidden="true"><g fill="#e8c04a">
  <polygon points="32,0 48,28 16,28"/><polygon points="16,28 32,56 0,56"/><polygon points="48,28 64,56 32,56"/></g></svg>
 <h1>RELOJ DE HYRULE</h1>
 <div class="reloj" id="hora">--:--:--</div>
 <div class="sub" id="fecha">Buscando la hora...</div>
 <div class="sub" id="red"></div>
 <div class="idiomas" role="group" aria-label="Idioma / Language">
  <button type="button" id="idioma-es" onclick="cambiarIdioma('es')">Español</button><button type="button" id="idioma-en" onclick="cambiarIdioma('en')">English</button>
 </div>
</header>

<section id="portal">
 <h2>¡Bienvenido, guardián del reloj!</h2>
 <p>El reloj todavía no está conectado al WiFi de casa. Elige tu red y escribe la clave para que aprenda la hora por internet.</p>
</section>

<form id="f" onsubmit="return false">

<section>
 <h2>Tu héroe</h2>
 <label>Nombre del niño o niña</label>
 <input type="text" name="nombre" maxlength="22" placeholder="Por ejemplo: ALEJANDRO">
 <div class="fila">
  <div><label>Día del cumpleaños</label><select name="cumpleDia" id="cumpleDia"></select></div>
  <div><label>Mes del cumpleaños</label><select name="cumpleMes" id="cumpleMes"></select></div>
 </div>
 <p class="nota" id="faltan"></p>
 <label>Mensaje secreto de papá y mamá (sale de vez en cuando)</label>
 <input type="text" name="mensaje" maxlength="94" placeholder="¡Te queremos mucho, {N}!">
 <p class="nota">Escribe {N} y el reloj pondrá el nombre del héroe.</p>
 <label class="check"><input type="checkbox" name="saludos"> Dar los buenos días y las buenas noches</label>
</section>

<section>
 <h2>La hora</h2>
 <label>Formato</label>
 <div class="opciones">
  <label><input type="radio" name="formato24" value="1"><span>24 horas (17:30)</span></label>
  <label><input type="radio" name="formato24" value="0"><span>12 horas (5:30 P)</span></label>
 </div>
 <p class="nota">En 12 horas sale una A (mañana, AM) o una P (tarde, PM) a la derecha.</p>
 <label class="check"><input type="checkbox" name="ceroIzquierda"> Cero delante en 24 h (09:05)</label>
 <label>Fecha</label>
 <select name="modoFecha">
  <option value="0">No enseñar la fecha</option>
  <option value="2">Enseñar "18/10" cada medio minuto</option>
  <option value="1">Decir "DOMINGO 18 DE OCTUBRE" de vez en cuando</option>
 </select>
 <label>Cada cuántos minutos se dice la fecha</label>
 <input type="number" name="fechaCadaMin" min="1" max="60">
 <label>Fila de abajo (los segundos)</label>
 <select name="modoSegundos">
  <option value="2">Un hada que vuela de lado a lado</option>
  <option value="1">Una barra que se llena cada minuto</option>
  <option value="0">Nada</option>
 </select>
 <label class="check"><input type="checkbox" name="parpadeo"> Los dos puntos parpadean</label>
 <label class="check"><input type="checkbox" name="horaEnPunto"> Celebrar las horas en punto</label>
</section>

<section>
 <h2>Aventuras</h2>
 <label>Cada cuánto aparece una aventura al azar</label>
 <select name="animCadaMin">
  <option value="0">Nunca</option><option value="3">Cada 3 minutos</option><option value="5">Cada 5 minutos</option>
  <option value="10">Cada 10 minutos</option><option value="15">Cada 15 minutos</option>
  <option value="20">Cada 20 minutos</option><option value="30">Cada 30 minutos</option><option value="60">Cada hora</option>
 </select>
 <label class="check"><input type="checkbox" name="frases"> Incluir frases de ánimo</label>
 <label>¡Lánzala ahora!</label>
 <div class="aventuras" id="aventuras"></div>
 <label>Escribir un mensaje en el reloj ahora mismo</label>
 <p class="nota"><a href="/mensaje" style="color:var(--oro)">📱 Página de mensajes para el móvil</a> (con personaje, duración y velocidad; se puede añadir a la pantalla de inicio)</p>
 <div class="fila"><input type="text" id="mensajeYa" maxlength="200" onkeydown="if(event.key==='Enter'){event.preventDefault();enviarMensajeYa();}" placeholder="¡A cenar, héroe!"><button type="button" style="flex:0" onclick="enviarMensajeYa()">Enviar</button></div>
</section>

<section>
 <h2>Modo noche</h2>
 <label class="check"><input type="checkbox" name="nocheActiva"> Activar modo noche (sin aventuras)</label>
 <div class="fila">
  <div><label>Desde</label><input type="time" id="nocheIni"></div>
  <div><label>Hasta</label><input type="time" id="nocheFin"></div>
 </div>
 <label class="check"><input type="checkbox" name="nocheApagar"> Apagar la pantalla del todo por la noche</label>
 <label>Brillo por la noche (si no se apaga): <b id="vBrilloNoche"></b></label>
 <input type="range" name="brilloNoche" min="0" max="15">
</section>

<section>
 <h2>Modo concentración</h2>
 <p class="nota">Para estudiar sin distracciones: solo la hora, quieta. Sin parpadeo, sin segundos, sin fecha, sin aventuras ni saludos. Los mensajes que envíes tú sí se ven.</p>
 <p><b id="concEstado"></b></p>
 <div class="botonesConc">
  <button type="button" class="sec" onclick="conc(25)">25 min</button><button type="button" class="sec" onclick="conc(45)">45 min</button><button type="button" class="sec" onclick="conc(60)">1 h</button><button type="button" class="sec" onclick="conc(120)">2 h</button><button type="button" class="sec" onclick="conc(-1)">Hasta que la quite</button><button type="button" class="peligro" onclick="conc(0)">Quitar</button>
 </div>
 <label class="check"><input type="checkbox" name="estudioActivo"> Horario de estudio (se pone solo)</label>
 <div class="fila">
  <div><label>Desde</label><input type="time" data-h="estudioIniH" data-m="estudioIniM"></div>
  <div><label>Hasta</label><input type="time" data-h="estudioFinH" data-m="estudioFinM"></div>
 </div>
 <div class="dias" data-campo="estudioDias"><label><input type="checkbox" data-bit="1"><span>L</span></label><label><input type="checkbox" data-bit="2"><span>M</span></label><label><input type="checkbox" data-bit="3"><span>X</span></label><label><input type="checkbox" data-bit="4"><span>J</span></label><label><input type="checkbox" data-bit="5"><span>V</span></label><label><input type="checkbox" data-bit="6"><span>S</span></label><label><input type="checkbox" data-bit="0"><span>D</span></label></div>
 <label class="check"><input type="checkbox" name="estudioAviso"> Al terminar: melodía y "¡BUEN TRABAJO!"</label>
</section>

<section>
 <h2>Sonido</h2>
 <p class="nota">El reloj solo suena cuando tú lo pides: un aviso con un mensaje o la alarma. Melodías originales de este proyecto.</p>
 <label class="check"><input type="checkbox" name="sonidoActivo"> Zumbador conectado</label>
 <label>Volumen: <b id="vVolumen"></b></label>
 <input type="range" name="volumen" min="1" max="10">
 <label>Pata del zumbador</label>
 <select name="pinSonido"><option value="0">D0</option><option value="1">D1</option><option value="2">D2</option><option value="3">D3</option><option value="4">D4</option><option value="5">D5</option><option value="6">D6</option><option value="7">D7</option></select>
 <p class="nota">Guarda los ajustes y prueba una melodía:</p>
 <div class="aventuras" id="melodias"></div>
</section>

<section>
 <h2>Alarma de la mañana</h2>
 <label class="check"><input type="checkbox" name="alarmaActiva"> Activar la alarma</label>
 <div class="fila">
  <div><label>Hora</label><input type="time" data-h="alarmaH" data-m="alarmaM"></div>
  <div><label>Melodía</label><select name="alarmaMelodia" id="alarmaMelodia"></select></div>
 </div>
 <div class="dias" data-campo="alarmaDias"><label><input type="checkbox" data-bit="1"><span>L</span></label><label><input type="checkbox" data-bit="2"><span>M</span></label><label><input type="checkbox" data-bit="3"><span>X</span></label><label><input type="checkbox" data-bit="4"><span>J</span></label><label><input type="checkbox" data-bit="5"><span>V</span></label><label><input type="checkbox" data-bit="6"><span>S</span></label><label><input type="checkbox" data-bit="0"><span>D</span></label></div>
 <label>Cuánto suena: <b id="vAlarmaSegundos"></b> s</label>
 <input type="range" name="alarmaSegundos" min="3" max="60">
 <label>Mensaje (vacío = "¡BUENOS DÍAS, {N}!")</label>
 <input type="text" name="alarmaTexto" maxlength="46" placeholder="¡BUENOS DÍAS, {N}!">
</section>

<section>
 <h2>Pantalla</h2>
 <label>Brillo de día: <b id="vBrillo"></b></label>
 <input type="range" name="brillo" min="0" max="15">
 <p class="nota">Con la ventana difusora delante, entre 5 y 8 se ve bien sin cansar la vista. De noche, mejor 0 o 1.</p>
 <label>Velocidad del texto: <b id="vVelocidad"></b> ms</label>
 <input type="range" name="velocidad" min="10" max="120">
 <p class="nota">Más número = más despacio (ideal para quien está aprendiendo a leer).</p>
 <details open><summary class="nota">Si las letras se ven giradas o desordenadas</summary>
  <label>Asistente de orientación</label>
  <p class="nota">Pulsa "Siguiente posición" hasta que cada cuadro enseñe su número derecho y bien leído (1, 2, 3, 4). Son 8 posiciones; cada una se guarda sola.</p>
  <div><button type="button" class="sec" onclick="asistente(-1)">◀ Anterior</button><button type="button" onclick="asistente(1)">Siguiente posición ▶</button></div>
  <p class="nota"><b id="asisEstado"></b></p>
  <p class="nota">Si los números salen bien pero en orden 4 3 2 1, marca "Invertir el orden de los módulos", guarda y vuelve a probar.</p>
  <label>Giro de cada módulo</label>
  <select name="rotacion"><option value="0">0°</option><option value="1">90°</option><option value="2">180°</option><option value="3">270°</option></select>
  <label class="check"><input type="checkbox" name="invertirModulos"> Invertir el orden de los módulos</label>
  <label class="check"><input type="checkbox" name="espejo"> Efecto espejo en cada módulo</label>
  <p class="nota">Guarda y pulsa "Probar pantalla": debe verse 1 2 3 4 de izquierda a derecha.</p>
 </details>
 <button type="button" class="sec" onclick="post('/api/prueba')">Probar pantalla</button>
</section>

<section>
 <h2>Hora por internet</h2>
 <label>Zona horaria</label>
 <select id="zonaLista" onchange="zonaElegida()">
  <option value="CET-1CEST,M3.5.0,M10.5.0/3">España (península y Baleares)</option>
  <option value="WET0WEST,M3.5.0/1,M10.5.0">España (Canarias)</option>
  <option value="CST6">México (centro)</option>
  <option value="&lt;-05&gt;5">Colombia / Perú / Ecuador</option>
  <option value="&lt;-03&gt;3">Argentina / Uruguay</option>
  <option value="&lt;-04&gt;4&lt;-03&gt;,M9.1.6/24,M4.1.6/24">Chile</option>
  <option value="&lt;-04&gt;4">Venezuela / Bolivia</option>
  <option value="EST5EDT,M3.2.0,M11.1.0">EE. UU. (este)</option>
  <option value="PST8PDT,M3.2.0,M11.1.0">EE. UU. (oeste)</option>
  <option value="otra">Otra (escribirla)</option>
 </select>
 <input type="text" name="zona" id="zona" style="margin-top:8px">
 <label>Servidor de la hora</label>
 <input type="text" name="servidorHora">
</section>

<p style="text-align:center"><button type="button" onclick="guardar()" style="font-size:1.15em;padding:12px 28px">Guardar ajustes</button></p>
</form>

<section>
 <h2>Diagnóstico de pantalla</h2>
 <p class="nota">Para encontrar averías en los cables o en la matriz. Mientras dura, el reloj no enseña la hora; se cierra solo a los 10 minutos.</p>
 <p class="nota"><b id="diagEstado"></b></p>
 <label>1. Encender los LED (con el modo de prueba de la propia matriz)</label>
 <div>
  <button type="button" onclick="diag('todos')">Encender todos los LED</button>
  <button type="button" class="sec" onclick="diag('modulo',1)">Módulo 1</button><button type="button" class="sec" onclick="diag('modulo',2)">Módulo 2</button><button type="button" class="sec" onclick="diag('modulo',3)">Módulo 3</button><button type="button" class="sec" onclick="diag('modulo',4)">Módulo 4</button>
 </div>
 <details><summary class="nota">¿Qué significa lo que veo?</summary>
  <p class="nota">Todos los LED encendidos: los cables y la matriz están bien. Si la hora se ve rara, es la orientación (apartado Pantalla).</p>
  <p class="nota">Solo se encienden los primeros módulos: la señal se corta al pasar al siguiente. Repasa las soldaduras entre ese módulo y el siguiente.</p>
  <p class="nota">No se enciende nada: la señal no llega. Mide las patas con el paso 2.</p>
  <p class="nota">El módulo 1 es el más cercano a los cables del Wemos.</p>
 </details>
 <label>2. Medir con el polímetro (entre GND y cada pata de la entrada de la matriz)</label>
 <div>
  <button type="button" class="sec" onclick="diag('pata',0)">DIN a 3,3 V</button><button type="button" class="sec" onclick="diag('pata',1)">CLK a 3,3 V</button><button type="button" class="sec" onclick="diag('pata',2)">CS a 3,3 V</button>
 </div>
 <p class="nota">La pata elegida debe marcar unos 3,3 V y las otras dos, 0 V. Si marca 0 V, falla el cable, la soldadura o esa pata del Wemos.</p>
 <button type="button" onclick="diag('fin')">Terminar diagnóstico</button>
 <form id="fPines" onsubmit="return false">
  <label>3. Patas del Wemos que usa la pantalla</label>
  <p class="nota">Si una pata del Wemos se ha estropeado, pasa ese cable a otra libre (D1 o D2) y elígela aquí. De fábrica: DIN = D7, CLK = D5, CS = D6.</p>
  <div class="fila">
   <div><label>DIN</label><select name="pinDin"><option value="0">D0</option><option value="1">D1</option><option value="2">D2</option><option value="3">D3</option><option value="4">D4</option><option value="5">D5</option><option value="6">D6</option><option value="7">D7</option></select></div>
   <div><label>CLK</label><select name="pinClk"><option value="0">D0</option><option value="1">D1</option><option value="2">D2</option><option value="3">D3</option><option value="4">D4</option><option value="5">D5</option><option value="6">D6</option><option value="7">D7</option></select></div>
   <div><label>CS</label><select name="pinCs"><option value="0">D0</option><option value="1">D1</option><option value="2">D2</option><option value="3">D3</option><option value="4">D4</option><option value="5">D5</option><option value="6">D6</option><option value="7">D7</option></select></div>
  </div>
  <label class="check"><input type="checkbox" name="spiLento"> Señales lentas (para cables largos o soldaduras dudosas)</label>
  <p class="nota">Ahora: <b id="pinesActual"></b></p>
  <button type="button" onclick="guardar('fPines')">Guardar</button>
 </form>
</section>

<section>
 <h2>WiFi de casa</h2>
 <p class="nota" id="wifiActual"></p>
 <button type="button" class="sec" onclick="buscarRedes()">Buscar redes WiFi</button>
 <div id="redes"></div>
 <label>Nombre de la red</label><input type="text" id="wNombre">
 <label>Clave</label><input type="password" id="wClave">
 <button type="button" onclick="conectarWifi()">Conectar y reiniciar</button>
</section>

<section>
 <h2>Actualizaciones</h2>
 <p><span>Versión instalada:</span> <b id="version"></b></p>
 <p class="nota" id="estadoAct"></p>
 <p class="nota" id="sistema"></p>
 <form id="f2" onsubmit="return false">
  <label>Dirección de actualizaciones (donde están version.txt y firmware.bin)</label>
  <input type="text" name="urlActualizacion" placeholder="https://wencescarlos.github.io/reloj-hyrule/">
  <label class="check"><input type="checkbox" name="autoActualizar"> Buscar e instalar versiones nuevas cada día</label>
  <label>Nueva clave de administrador (déjala vacía para no cambiarla)</label>
  <input type="password" name="claveAdmin" placeholder="••••••">
  <button type="button" onclick="guardar('f2')">Guardar</button>
 </form>
 <button type="button" class="sec" onclick="post('/api/buscar-version',t('Buscando versión nueva...','Checking for a new version...'))">Buscar versión nueva ahora</button>
 <a class="boton" href="/actualizar">Subir archivo .bin</a>
 <p class="nota">Para subir un archivo: usuario <b>admin</b> y tu clave de administrador (de fábrica: hyrule).</p>
 <hr style="border-color:#2c4a37">
 <button type="button" class="sec" onclick="post('/api/reiniciar',t('Reiniciando...','Restarting...'))">Reiniciar el reloj</button>
 <button type="button" class="peligro" onclick="if(confirm(t('¿Borrar todos los ajustes?','Erase all settings?')))post('/api/fabrica',t('Volviendo a los ajustes de fábrica...','Restoring factory settings...'))">Ajustes de fábrica</button>
</section>

<footer><span data-l="es">Proyecto de fans <b>sin ánimo de lucro</b>, hecho con cariño como homenaje a <i>The Legend of Zelda</i>.<br>
No oficial y sin relación con Nintendo. The Legend of Zelda es una marca de Nintendo.<br>
Prohibida su venta. Código y dibujos originales: github.com/wencescarlos/reloj-hyrule<br>
Página del reloj: http://reloj-hyrule.local</span><span data-l="en"><b>Non-profit</b> fan project, made with love as a tribute to <i>The Legend of Zelda</i>.<br>
Unofficial and not affiliated with Nintendo. The Legend of Zelda is a trademark of Nintendo.<br>
Not for sale. Original code and pixel art: github.com/wencescarlos/reloj-hyrule<br>
Clock page: http://reloj-hyrule.local</span></footer>
</main>
<div id="aviso"></div>

<script>
const $=s=>document.querySelector(s);
// ---------------------------------------------------------------- idioma
// Textos en inglés de la página (la clave es el texto en español)
const EN={
"Reloj de Hyrule":"Hyrule Clock","RELOJ DE HYRULE":"HYRULE CLOCK","Buscando la hora...":"Looking for the time...",
"¡Bienvenido, guardián del reloj!":"Welcome, keeper of the clock!",
"El reloj todavía no está conectado al WiFi de casa. Elige tu red y escribe la clave para que aprenda la hora por internet.":"The clock is not connected to your home WiFi yet. Choose your network and type the password so it can learn the time from the internet.",
"Tu héroe":"Your hero","Nombre del niño o niña":"Child's name","Día del cumpleaños":"Birthday (day)","Mes del cumpleaños":"Birthday (month)",
"Mensaje secreto de papá y mamá (sale de vez en cuando)":"Secret message from mum and dad (shows up now and then)",
"Escribe {N} y el reloj pondrá el nombre del héroe.":"Type {N} and the clock will put the hero's name there.",
"Dar los buenos días y las buenas noches":"Say good morning and good night",
"La hora":"The time","Formato":"Format","24 horas (17:30)":"24 hours (17:30)","12 horas (5:30 P)":"12 hours (5:30 P)",
"En 12 horas sale una A (mañana, AM) o una P (tarde, PM) a la derecha.":"In 12-hour mode an A (morning, AM) or a P (afternoon, PM) shows on the right.",
"Cero delante en 24 h (09:05)":"Leading zero in 24 h (09:05)","Fecha":"Date","No enseñar la fecha":"Don't show the date",
"Enseñar \"18/10\" cada medio minuto":"Show \"18/10\" every half minute",
"Decir \"DOMINGO 18 DE OCTUBRE\" de vez en cuando":"Scroll \"SUNDAY, OCTOBER 18\" now and then",
"Cada cuántos minutos se dice la fecha":"How often the date scrolls (minutes)","Fila de abajo (los segundos)":"Bottom row (the seconds)",
"Un hada que vuela de lado a lado":"A fairy flying from side to side","Una barra que se llena cada minuto":"A bar that fills up every minute","Nada":"Nothing",
"Los dos puntos parpadean":"The colon blinks","Celebrar las horas en punto":"Celebrate every hour on the hour",
"Aventuras":"Adventures","Cada cuánto aparece una aventura al azar":"How often a random adventure appears","Nunca":"Never",
"Cada 3 minutos":"Every 3 minutes","Cada 5 minutos":"Every 5 minutes","Cada 10 minutos":"Every 10 minutes","Cada 15 minutos":"Every 15 minutes",
"Cada 20 minutos":"Every 20 minutes","Cada 30 minutos":"Every 30 minutes","Cada hora":"Every hour",
"Incluir frases de ánimo":"Include words of courage","¡Lánzala ahora!":"Launch one now!",
"Escribir un mensaje en el reloj ahora mismo":"Write a message on the clock right now","Enviar":"Send","¡A cenar, héroe!":"Dinner time, hero!",
"Modo noche":"Night mode","Activar modo noche (sin aventuras)":"Turn on night mode (no adventures)","Desde":"From","Hasta":"To",
"Apagar la pantalla del todo por la noche":"Turn the display off completely at night",
"Brillo por la noche (si no se apaga):":"Night brightness (if not off):",
"Pantalla":"Display","Brillo de día:":"Daytime brightness:","Con la ventana difusora delante, entre 5 y 8 se ve bien sin cansar la vista. De noche, mejor 0 o 1.":"With the diffuser window in front, 5 to 8 looks good without tiring the eyes. At night, 0 or 1 is best.",
"Velocidad del texto:":"Text speed:","Más número = más despacio (ideal para quien está aprendiendo a leer).":"Higher number = slower (great for kids learning to read).",
"Si las letras se ven giradas o desordenadas":"If the letters look rotated or jumbled","Giro de cada módulo":"Rotation of each module",
"Invertir el orden de los módulos":"Reverse the order of the modules","Efecto espejo en cada módulo":"Mirror each module",
"Guarda y pulsa \"Probar pantalla\": debe verse 1 2 3 4 de izquierda a derecha.":"Save and press \"Test display\": you should see 1 2 3 4 from left to right.",
"Probar pantalla":"Test display","Hora por internet":"Internet time","Zona horaria":"Time zone",
"España (península y Baleares)":"Spain (mainland and Balearic Islands)","España (Canarias)":"Spain (Canary Islands)","México (centro)":"Mexico (central)",
"EE. UU. (este)":"USA (Eastern)","EE. UU. (oeste)":"USA (Pacific)","Otra (escribirla)":"Other (type it in)",
"Servidor de la hora":"Time server","Guardar ajustes":"Save settings",
"WiFi de casa":"Home WiFi","Buscar redes WiFi":"Search for WiFi networks","Nombre de la red":"Network name","Clave":"Password",
"Conectar y reiniciar":"Connect and restart","Actualizaciones":"Updates","Versión instalada:":"Installed version:",
"Dirección de actualizaciones (donde están version.txt y firmware.bin)":"Update address (where version.txt and firmware.bin are)",
"Buscar e instalar versiones nuevas cada día":"Check for and install new versions every day",
"Nueva clave de administrador (déjala vacía para no cambiarla)":"New admin password (leave it empty to keep the current one)",
"Guardar":"Save","Buscar versión nueva ahora":"Check for a new version now","Subir archivo .bin":"Upload a .bin file",
"Para subir un archivo: usuario":"To upload a file: user","y tu clave de administrador (de fábrica: hyrule).":"and your admin password (factory default: hyrule).",
"Reiniciar el reloj":"Restart the clock",
"Modo concentración":"Focus mode","Para estudiar sin distracciones: solo la hora, quieta. Sin parpadeo, sin segundos, sin fecha, sin aventuras ni saludos. Los mensajes que envíes tú sí se ven.":"To study without distractions: just the time, still. No blinking, no seconds, no date, no adventures or greetings. Messages you send still show up.",
"Hasta que la quite":"Until I turn it off","Quitar":"Turn off","Horario de estudio (se pone solo)":"Study schedule (turns on by itself)",
"Al terminar: melodía y \"¡BUEN TRABAJO!\"":"When it ends: a tune and \"WELL DONE!\"",
"Sonido":"Sound","El reloj solo suena cuando tú lo pides: un aviso con un mensaje o la alarma. Melodías originales de este proyecto.":"The clock only makes sound when you ask: a message alert or the alarm. Original tunes made for this project.",
"Zumbador conectado":"Buzzer connected","Volumen:":"Volume:","Pata del zumbador":"Buzzer pin","Guarda los ajustes y prueba una melodía:":"Save the settings and try a tune:",
"Alarma de la mañana":"Morning alarm","Activar la alarma":"Turn the alarm on","Hora":"Time","Melodía":"Tune","Cuánto suena:":"How long it plays:",
"Mensaje (vacío = \"¡BUENOS DÍAS, {N}!\")":"Message (empty = \"GOOD MORNING, {N}!\")","📱 Página de mensajes para el móvil":"📱 Message page for your phone","(con personaje, duración y velocidad; se puede añadir a la pantalla de inicio)":"(with a character, duration and speed; you can add it to your home screen)","Ajustes de fábrica":"Factory reset",
"Diagnóstico de pantalla":"Display diagnostics",
"Para encontrar averías en los cables o en la matriz. Mientras dura, el reloj no enseña la hora; se cierra solo a los 10 minutos.":"To find faults in the wires or the matrix. While it runs, the clock doesn't show the time; it ends by itself after 10 minutes.",
"1. Encender los LED (con el modo de prueba de la propia matriz)":"1. Light up the LEDs (using the matrix's own test mode)",
"Encender todos los LED":"Light all LEDs","Módulo 1":"Module 1","Módulo 2":"Module 2","Módulo 3":"Module 3","Módulo 4":"Module 4",
"¿Qué significa lo que veo?":"What does it mean?",
"Todos los LED encendidos: los cables y la matriz están bien. Si la hora se ve rara, es la orientación (apartado Pantalla).":"All LEDs on: the wires and the matrix are fine. If the time looks odd, it's the orientation (Display section).",
"Solo se encienden los primeros módulos: la señal se corta al pasar al siguiente. Repasa las soldaduras entre ese módulo y el siguiente.":"Only the first modules light up: the signal stops at the next one. Check the solder joints between that module and the next.",
"No se enciende nada: la señal no llega. Mide las patas con el paso 2.":"Nothing lights up: the signal isn't getting through. Measure the pins in step 2.",
"El módulo 1 es el más cercano a los cables del Wemos.":"Module 1 is the one nearest the Wemos wires.",
"2. Medir con el polímetro (entre GND y cada pata de la entrada de la matriz)":"2. Measure with a multimeter (between GND and each pin of the matrix input)",
"DIN a 3,3 V":"DIN at 3.3 V","CLK a 3,3 V":"CLK at 3.3 V","CS a 3,3 V":"CS at 3.3 V",
"La pata elegida debe marcar unos 3,3 V y las otras dos, 0 V. Si marca 0 V, falla el cable, la soldadura o esa pata del Wemos.":"The chosen pin should read about 3.3 V and the other two 0 V. If it reads 0 V, the wire, the solder joint or that Wemos pin is faulty.",
"Terminar diagnóstico":"End diagnostics","3. Patas del Wemos que usa la pantalla":"3. Wemos pins used by the display",
"Si una pata del Wemos se ha estropeado, pasa ese cable a otra libre (D1 o D2) y elígela aquí. De fábrica: DIN = D7, CLK = D5, CS = D6.":"If a Wemos pin is damaged, move that wire to a free one (D1 or D2) and choose it here. Factory setting: DIN = D7, CLK = D5, CS = D6.",
"Señales lentas (para cables largos o soldaduras dudosas)":"Slow signals (for long wires or doubtful solder joints)",
"Asistente de orientación":"Orientation wizard",
"Pulsa \"Siguiente posición\" hasta que cada cuadro enseñe su número derecho y bien leído (1, 2, 3, 4). Son 8 posiciones; cada una se guarda sola.":"Press \"Next position\" until each square shows its number upright and readable (1, 2, 3, 4). There are 8 positions; each one is saved automatically.",
"◀ Anterior":"◀ Previous","Siguiente posición ▶":"Next position ▶",
"Si los números salen bien pero en orden 4 3 2 1, marca \"Invertir el orden de los módulos\", guarda y vuelve a probar.":"If the numbers look right but read 4 3 2 1, tick \"Reverse the order of the modules\", save and test again.","Ahora:":"Now:"
};
const MESES={es:["enero","febrero","marzo","abril","mayo","junio","julio","agosto","septiembre","octubre","noviembre","diciembre"],
 en:["January","February","March","April","May","June","July","August","September","October","November","December"]};
let L="es",traducibles=null;
function t(es,en){return L==="en"?en:es;}

// Recoge una sola vez los textos, opciones y "placeholders" que tienen traducción
function recoger(){
 traducibles=[];
 const w=document.createTreeWalker(document.body,NodeFilter.SHOW_TEXT);let n;
 while(n=w.nextNode()){const k=n.nodeValue.trim();if(k&&EN[k])traducibles.push({n,orig:n.nodeValue,k});}
 document.querySelectorAll("[placeholder]").forEach(e=>{const k=e.placeholder;if(EN[k])traducibles.push({n:e,orig:k,k,attr:"placeholder"});});
}
function aplicarIdioma(){
 if(!traducibles)recoger();
 document.documentElement.lang=L;
 traducibles.forEach(x=>{const v=L==="en"?(x.attr?EN[x.k]:x.orig.replace(x.k,EN[x.k])):x.orig;if(x.attr)x.n[x.attr]=v;else x.n.nodeValue=v;});
 document.title=t("Reloj de Hyrule","Hyrule Clock");
 $("[name=nombre]").placeholder=t("Por ejemplo: ALEJANDRO","For example: ALEJANDRO");
 $("[name=mensaje]").placeholder=t("¡Te queremos mucho, {N}!","We love you so much, {N}!");
 $("#idioma-es").classList.toggle("activo",L==="es");$("#idioma-en").classList.toggle("activo",L==="en");
 const m=$("#cumpleMes");MESES[L].forEach((n,i)=>m.options[i+1].text=n);
 const DIAS={es:["D","L","M","X","J","V","S"],en:["S","M","T","W","T","F","S"]};
 document.querySelectorAll(".dias input").forEach(c=>c.nextElementSibling.textContent=DIAS[L][c.dataset.bit]);
}
function cambiarIdioma(l){
 if(l===L)return;L=l;aplicarIdioma();
 post("/api/ajustes",t("Ahora el reloj habla español","The clock now speaks English"),new URLSearchParams({idioma:L==="en"?"1":"0"}))
  .then(()=>{cargarAventuras();estado();});
}

// ---------------------------------------------------------------- utilidades
function mostrarAviso(x){const a=$("#aviso");a.textContent=x;a.classList.add("ver");clearTimeout(a.t);a.t=setTimeout(()=>a.classList.remove("ver"),2500);}
function post(u,x,datos){return fetch(u,{method:"POST",body:datos||new URLSearchParams()}).then(r=>{if(r.ok)mostrarAviso(x||t("¡Hecho!","Done!"));else mostrarAviso(t("Algo ha fallado","Something went wrong"));return r;}).catch(()=>mostrarAviso(t("Sin conexión con el reloj","No connection to the clock")));}
function dos(n){return String(n).padStart(2,"0");}

// Rellenar los desplegables del cumpleaños
let d=$("#cumpleDia");d.add(new Option("—","0"));for(let i=1;i<=31;i++)d.add(new Option(i,i));
let m=$("#cumpleMes");m.add(new Option("—","0"));MESES.es.forEach((n,i)=>m.add(new Option(n,i+1)));

function rangos(){[["brillo","vBrillo"],["brilloNoche","vBrilloNoche"],["velocidad","vVelocidad"],["volumen","vVolumen"],["alarmaSegundos","vAlarmaSegundos"]].forEach(([n,v])=>{
 const e=document.querySelector("[name="+n+"]");$("#"+v).textContent=e.value;e.oninput=()=>$("#"+v).textContent=e.value;});}

function zonaElegida(){const v=$("#zonaLista").value;if(v!=="otra")$("#zona").value=v;$("#zona").style.display=v==="otra"?"block":"none";}

function cargarAventuras(){
 fetch("/api/aventuras").then(r=>r.json()).then(l=>{
  const c=$("#aventuras");c.innerHTML="";
  const s=document.createElement("button");s.type="button";s.textContent=t("✨ ¡Sorpresa!","✨ Surprise!");
  s.onclick=()=>post("/api/animacion",t("¡Allá va una sorpresa!","Here comes a surprise!"));c.appendChild(s);
  l.forEach((n,i)=>{const b=document.createElement("button");b.type="button";b.className="sec";b.textContent=n;
   b.onclick=()=>post("/api/animacion?n="+i,t("¡Allá va: "+n+"!","Here it comes: "+n+"!"));c.appendChild(b);});
 });
}

function cargar(){
 fetch("/api/ajustes").then(r=>r.json()).then(a=>{
  document.querySelectorAll("[name]").forEach(e=>{
   if(!(e.name in a))return;const v=a[e.name];
   if(e.type==="checkbox")e.checked=v==1;else if(e.type==="radio")e.checked=e.value==v;else e.value=v;});
  $("#nocheIni").value=dos(a.nocheIniH)+":"+dos(a.nocheIniM);
  document.querySelectorAll("[data-h]").forEach(e=>{if(e.dataset.h in a)e.value=dos(a[e.dataset.h])+":"+dos(a[e.dataset.m]);});
  document.querySelectorAll(".dias").forEach(d=>{const v=a[d.dataset.campo]||0;d.querySelectorAll("input").forEach(c=>c.checked=!!(v>>c.dataset.bit&1));});
  cargarMelodias(a);
  $("#nocheFin").value=dos(a.nocheFinH)+":"+dos(a.nocheFinM);
  const zl=$("#zonaLista");zl.value=[...zl.options].some(o=>o.value===a.zona)?a.zona:"otra";zonaElegida();
  rangos();
  L=a.idioma==1?"en":"es";aplicarIdioma();
  cargarAventuras();
 });
}

function guardar(id){
 const f=$("#"+(id||"f"));const p=new URLSearchParams();
 f.querySelectorAll("[name]").forEach(e=>{
  if(e.type==="checkbox")p.append(e.name,e.checked?"1":"0");
  else if(e.type==="radio"){if(e.checked)p.append(e.name,e.value);}
  else p.append(e.name,e.value);});
 f.querySelectorAll("[data-h]").forEach(e=>{if(!e.value)return;const [h,m]=e.value.split(":");p.append(e.dataset.h,+h);p.append(e.dataset.m,+m);});
 f.querySelectorAll(".dias").forEach(d=>{let v=0;d.querySelectorAll("input").forEach(c=>{if(c.checked)v|=1<<c.dataset.bit;});p.append(d.dataset.campo,v);});
 if(!id){const [ih,im]=$("#nocheIni").value.split(":"),[fh,fm]=$("#nocheFin").value.split(":");
  p.append("nocheIniH",+ih);p.append("nocheIniM",+im);p.append("nocheFinH",+fh);p.append("nocheFinM",+fm);}
 post("/api/ajustes",t("¡Ajustes guardados!","Settings saved!"),p);
}

function diag(a,n){post("/api/diagnostico?accion="+a+(n!==undefined?"&n="+n:""),a==="fin"?t("Diagnóstico terminado","Diagnostics ended"):t("¡Hecho! Mira la pantalla","Done! Look at the display"));}

// Asistente de orientación: recorre giro (4) x espejo (2) y lanza la prueba
let asisPos=-1;
function asistente(d){
 if(asisPos<0){const r=+$("[name=rotacion]").value,e=$("[name=espejo]").checked?1:0;asisPos=e*4+r;}
 asisPos=(asisPos+d+8)%8;const r=asisPos%4,e=asisPos>=4?1:0;
 $("[name=rotacion]").value=r;$("[name=espejo]").checked=!!e;
 const inv=$("[name=invertirModulos]").checked?"1":"0";
 $("#asisEstado").textContent=t("Posición ","Position ")+(asisPos+1)+t(" de 8: giro "," of 8: rotation ")+(r*90)+"°, "+t("espejo ","mirror ")+(e?t("sí","yes"):"no");
 fetch("/api/ajustes",{method:"POST",body:new URLSearchParams({rotacion:r,espejo:e,invertirModulos:inv})})
  .then(()=>post("/api/prueba?corta=1",t("Mira la pantalla","Look at the display")));
}

function conc(m){post("/api/concentracion?minutos="+m,m?t("Modo concentración puesto","Focus mode on"):t("Modo concentración quitado","Focus mode off")).then(()=>setTimeout(estado,300));}
function cargarMelodias(a){fetch("/api/melodias").then(r=>r.json()).then(l=>{
 const c=$("#melodias");c.innerHTML="";const s=$("#alarmaMelodia");s.innerHTML="";
 l.forEach((n,i)=>{const b=document.createElement("button");b.type="button";b.className="sec";b.textContent="♪ "+n;
  b.onclick=()=>fetch("/api/sonido?melodia="+i,{method:"POST"}).then(r=>mostrarAviso(r.ok?t("♪ Sonando: ","♪ Playing: ")+n:t("Activa el zumbador y guarda los ajustes","Turn the buzzer on and save the settings")));c.appendChild(b);
  s.appendChild(new Option(n,i));});
 const p=document.createElement("button");p.type="button";p.className="peligro";p.textContent=t("■ Parar","■ Stop");
 p.onclick=()=>fetch("/api/sonido",{method:"POST"});c.appendChild(p);
 if(a)s.value=a.alarmaMelodia;}).catch(()=>{});}
function tiempoTexto(s){if(s<0)return t("hasta que la quites","until you turn it off");const h=Math.floor(s/3600),m=Math.ceil((s%3600)/60);return (h?h+" h ":"")+m+" min";}

function enviarMensajeYa(){const x=$("#mensajeYa").value.trim();if(!x)return;
 post("/api/mensaje",t("¡Mensaje enviado!","Message sent!"),new URLSearchParams({texto:x}));$("#mensajeYa").value="";}

function buscarRedes(){$("#redes").textContent=t("Buscando...","Searching...");
 fetch("/api/redes").then(r=>r.json()).then(l=>{const c=$("#redes");c.innerHTML="";
  if(!l.length)c.textContent=t("No se ha encontrado ninguna red.","No networks found.");
  l.sort((a,b)=>b.senal-a.senal).forEach(r=>{const e=document.createElement("div");
   e.textContent=(r.abierta?"🔓 ":"🔒 ")+r.nombre+"  ("+r.senal+" dBm)";e.onclick=()=>{$("#wNombre").value=r.nombre;$("#wClave").focus();};c.appendChild(e);});});}

function conectarWifi(){const n=$("#wNombre").value.trim();if(!n){mostrarAviso(t("Escribe el nombre de la red","Type the network name"));return;}
 post("/api/wifi",t("Conectando... busca el reloj en http://reloj-hyrule.local","Connecting... find the clock at http://reloj-hyrule.local"),new URLSearchParams({nombre:n,clave:$("#wClave").value}));}

function fechaBonita(f){f=f.toLowerCase();return L==="en"?f.replace(/\b[a-z]/g,c=>c.toUpperCase()):f;}

function estado(){
 fetch("/api/estado").then(r=>r.json()).then(e=>{
  $("#hora").textContent=e.hora;
  $("#fecha").textContent=e.sincronizado?fechaBonita(e.fecha):t("Buscando la hora en internet...","Getting the time from the internet...");
  $("#red").textContent=e.conectado?("WiFi: "+e.wifi+" · "+e.ip):t("Sin WiFi de casa","No home WiFi");
  $("#portal").style.display=e.portal?"block":"none";
  $("#wifiActual").textContent=e.conectado?t("Conectado a «"+e.wifi+"» (señal "+e.senal+" dBm)","Connected to “"+e.wifi+"” (signal "+e.senal+" dBm)"):t("No conectado.","Not connected.");
  $("#concEstado").textContent=e.concentracion?("🎓 "+t("Activo","On")+(e.concHorario?t(" (horario de estudio)"," (study schedule)"):"")+" · "+t("queda ","left: ")+tiempoTexto(e.concQueda)):t("Ahora no está activo.","Not active right now.");
  $("#version").textContent=e.version;
  const R={"Power On":t("encendido","power on"),"External System":t("botón de reinicio","reset button"),"Software/System restart":t("reinicio normal (ajustes o actualización)","normal restart (settings or update)"),
   "Exception":t("⚠️ fallo del programa","⚠️ program crash"),"Hardware Watchdog":t("⚠️ el programa se quedó bloqueado","⚠️ the program froze"),"Software Watchdog":t("⚠️ el programa se quedó bloqueado","⚠️ the program froze"),"Deep-Sleep Wake":t("despertar","wake up")};
  const enc=e.encendido||0;
  $("#sistema").textContent=t("Último reinicio: ","Last restart: ")+(R[e.reinicio]||e.reinicio||"?")+" · "+t("encendido hace ","on for ")+(enc>=3600?Math.floor(enc/3600)+" h ":"")+Math.floor(enc%3600/60)+" min · "+t("memoria libre: ","free memory: ")+e.memoria+" B";$("#pinesActual").textContent=e.pines||"";
  $("#diagEstado").textContent=e.diagnostico?("🔧 "+e.diagnostico):"";$("#estadoAct").textContent=e.actualizacion;
  $("#faltan").textContent=e.diasCumple>0?t("¡Faltan "+e.diasCumple+" días para el cumpleaños!",(e.diasCumple==1?"Only 1 day":"Only "+e.diasCumple+" days")+" until the birthday!")
   :(e.diasCumple==0?t("¡HOY ES EL CUMPLEAÑOS! 🎂","IT'S BIRTHDAY TODAY! 🎂"):"");
 }).catch(()=>{});
}
aplicarIdioma();cargar();estado();setInterval(estado,1000);
</script>
</body></html>)PAGINA";
