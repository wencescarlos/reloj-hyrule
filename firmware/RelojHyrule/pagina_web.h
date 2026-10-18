// ===========================================================================
//  pagina_web.h  -  Portal de configuración del Reloj de Hyrule
//  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
//  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
//  Una sola página (HTML + CSS + JavaScript) guardada en la memoria flash.
//  Funciona sin internet: no usa nada de fuera.
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
footer{text-align:center;color:var(--suave);font-size:.8em;margin:24px 0 40px}
</style></head><body><main>

<header>
 <svg viewBox="0 0 64 56" aria-hidden="true"><g fill="#e8c04a">
  <polygon points="32,0 48,28 16,28"/><polygon points="16,28 32,56 0,56"/><polygon points="48,28 64,56 32,56"/></g></svg>
 <h1>RELOJ DE HYRULE</h1>
 <div class="reloj" id="hora">--:--:--</div>
 <div class="sub" id="fecha">Buscando la hora...</div>
 <div class="sub" id="red"></div>
</header>

<section id="portal">
 <h2>¡Bienvenido, guardián del reloj!</h2>
 <p>El reloj todavía no está conectado al WiFi de casa. Elige tu red y escribe la clave para que aprenda la hora por internet.</p>
</section>

<form id="f" onsubmit="return false">

<section>
 <h2>Tu héroe</h2>
 <label>Nombre del niño o niña</label>
 <input type="text" name="nombre" maxlength="22" placeholder="Por ejemplo: LUCAS">
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
 <div class="fila"><input type="text" id="mensajeYa" maxlength="200" placeholder="¡A cenar, héroe!"><button type="button" style="flex:0" onclick="mensajeYa()">Enviar</button></div>
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
 <h2>Pantalla</h2>
 <label>Brillo de día: <b id="vBrillo"></b></label>
 <input type="range" name="brillo" min="0" max="15">
 <p class="nota">Para que no canse la vista, mejor entre 1 y 4.</p>
 <label>Velocidad del texto: <b id="vVelocidad"></b> ms</label>
 <input type="range" name="velocidad" min="10" max="120">
 <p class="nota">Más número = más despacio (ideal para quien está aprendiendo a leer).</p>
 <details><summary class="nota">Si las letras se ven giradas o desordenadas</summary>
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
 <p>Versión instalada: <b id="version"></b></p>
 <p class="nota" id="estadoAct"></p>
 <form id="f2" onsubmit="return false">
  <label>Dirección de actualizaciones (donde están version.txt y firmware.bin)</label>
  <input type="text" name="urlActualizacion" placeholder="https://wencescarlos.github.io/reloj-hyrule/">
  <label class="check"><input type="checkbox" name="autoActualizar"> Buscar e instalar versiones nuevas cada día</label>
  <label>Nueva clave de administrador (déjala vacía para no cambiarla)</label>
  <input type="password" name="claveAdmin" placeholder="••••••">
  <button type="button" onclick="guardar('f2')">Guardar</button>
 </form>
 <button type="button" class="sec" onclick="post('/api/buscar-version','Buscando versión nueva...')">Buscar versión nueva ahora</button>
 <a class="boton" href="/actualizar">Subir archivo .bin</a>
 <p class="nota">Para subir un archivo: usuario <b>admin</b> y tu clave de administrador (de fábrica: hyrule).</p>
 <hr style="border-color:#2c4a37">
 <button type="button" class="sec" onclick="post('/api/reiniciar','Reiniciando...')">Reiniciar el reloj</button>
 <button type="button" class="peligro" onclick="if(confirm('¿Borrar todos los ajustes?'))post('/api/fabrica','Volviendo a los ajustes de fábrica...')">Ajustes de fábrica</button>
</section>

<footer>Proyecto de fans <b>sin ánimo de lucro</b>, hecho con cariño como homenaje a <i>The Legend of Zelda</i>.<br>
No oficial y sin relación con Nintendo. The Legend of Zelda es una marca de Nintendo.<br>
Prohibida su venta. Código y dibujos originales: github.com/wencescarlos/reloj-hyrule<br>
Página del reloj: http://reloj-hyrule.local</footer>
</main>
<div id="aviso"></div>

<script>
const $=s=>document.querySelector(s);
const MESES=["enero","febrero","marzo","abril","mayo","junio","julio","agosto","septiembre","octubre","noviembre","diciembre"];
function aviso(t){const a=$("#aviso");a.textContent=t;a.classList.add("ver");clearTimeout(a.t);a.t=setTimeout(()=>a.classList.remove("ver"),2500);}
function post(u,t,datos){return fetch(u,{method:"POST",body:datos||new URLSearchParams()}).then(r=>{if(r.ok)aviso(t||"¡Hecho!");else aviso("Algo ha fallado");return r;}).catch(()=>aviso("Sin conexión con el reloj"));}
function dos(n){return String(n).padStart(2,"0");}

// Rellenar los desplegables del cumpleaños
let d=$("#cumpleDia");d.add(new Option("—","0"));for(let i=1;i<=31;i++)d.add(new Option(i,i));
let m=$("#cumpleMes");m.add(new Option("—","0"));MESES.forEach((n,i)=>m.add(new Option(n,i+1)));

function rangos(){[["brillo","vBrillo"],["brilloNoche","vBrilloNoche"],["velocidad","vVelocidad"]].forEach(([n,v])=>{
 const e=document.querySelector("[name="+n+"]");$("#"+v).textContent=e.value;e.oninput=()=>$("#"+v).textContent=e.value;});}

function zonaElegida(){const v=$("#zonaLista").value;if(v!=="otra")$("#zona").value=v;$("#zona").style.display=v==="otra"?"block":"none";}

function cargar(){
 fetch("/api/ajustes").then(r=>r.json()).then(a=>{
  document.querySelectorAll("[name]").forEach(e=>{
   if(!(e.name in a))return;const v=a[e.name];
   if(e.type==="checkbox")e.checked=v==1;else if(e.type==="radio")e.checked=e.value==v;else e.value=v;});
  $("#nocheIni").value=dos(a.nocheIniH)+":"+dos(a.nocheIniM);
  $("#nocheFin").value=dos(a.nocheFinH)+":"+dos(a.nocheFinM);
  const zl=$("#zonaLista");zl.value=[...zl.options].some(o=>o.value===a.zona)?a.zona:"otra";zonaElegida();
  rangos();
 });
 fetch("/api/aventuras").then(r=>r.json()).then(l=>{
  const c=$("#aventuras");c.innerHTML="";
  const s=document.createElement("button");s.type="button";s.textContent="✨ ¡Sorpresa!";s.onclick=()=>post("/api/animacion","¡Allá va una sorpresa!");c.appendChild(s);
  l.forEach((n,i)=>{const b=document.createElement("button");b.type="button";b.className="sec";b.textContent=n;
   b.onclick=()=>post("/api/animacion?n="+i,"¡Allá va: "+n+"!");c.appendChild(b);});
 });
}

function guardar(id){
 const f=$("#"+(id||"f"));const p=new URLSearchParams();
 f.querySelectorAll("[name]").forEach(e=>{
  if(e.type==="checkbox")p.append(e.name,e.checked?"1":"0");
  else if(e.type==="radio"){if(e.checked)p.append(e.name,e.value);}
  else p.append(e.name,e.value);});
 if(!id){const [ih,im]=$("#nocheIni").value.split(":"),[fh,fm]=$("#nocheFin").value.split(":");
  p.append("nocheIniH",+ih);p.append("nocheIniM",+im);p.append("nocheFinH",+fh);p.append("nocheFinM",+fm);}
 post("/api/ajustes","¡Ajustes guardados!",p);
}

function mensajeYa(){const t=$("#mensajeYa").value.trim();if(!t)return;
 post("/api/mensaje","¡Mensaje enviado!",new URLSearchParams({texto:t}));$("#mensajeYa").value="";}

function buscarRedes(){$("#redes").textContent="Buscando...";
 fetch("/api/redes").then(r=>r.json()).then(l=>{const c=$("#redes");c.innerHTML="";
  if(!l.length)c.textContent="No se ha encontrado ninguna red.";
  l.sort((a,b)=>b.senal-a.senal).forEach(r=>{const e=document.createElement("div");
   e.textContent=(r.abierta?"🔓 ":"🔒 ")+r.nombre+"  ("+r.senal+" dBm)";e.onclick=()=>{$("#wNombre").value=r.nombre;$("#wClave").focus();};c.appendChild(e);});});}

function conectarWifi(){const n=$("#wNombre").value.trim();if(!n){aviso("Escribe el nombre de la red");return;}
 post("/api/wifi","Conectando... busca el reloj en http://reloj-hyrule.local",new URLSearchParams({nombre:n,clave:$("#wClave").value}));}

function estado(){
 fetch("/api/estado").then(r=>r.json()).then(e=>{
  $("#hora").textContent=e.hora;
  $("#fecha").textContent=e.sincronizado?e.fecha.toLowerCase():"Buscando la hora en internet...";
  $("#red").textContent=e.conectado?("WiFi: "+e.wifi+" · "+e.ip):"Sin WiFi de casa";
  $("#portal").style.display=e.portal?"block":"none";
  $("#wifiActual").textContent=e.conectado?("Conectado a «"+e.wifi+"» (señal "+e.senal+" dBm)"):"No conectado.";
  $("#version").textContent=e.version;$("#estadoAct").textContent=e.actualizacion;
  $("#faltan").textContent=e.diasCumple>0?("¡Faltan "+e.diasCumple+" días para el cumpleaños!"):(e.diasCumple==0?"¡HOY ES EL CUMPLEAÑOS! 🎂":"");
 }).catch(()=>{});
}
cargar();estado();setInterval(estado,1000);
</script>
</body></html>)PAGINA";
