/* =========================================================
  Orquesta 
    
  Librería tocar canciones con un motor de paso + DRV8825.

  Por: Laboratorio EnFlujo https://enflujo.com
   ========================================================= */

/// ---------- CONEXIÓN ARDUINO - DRV8825 ----------
const uint8_t PIN_PASO = 3;   // STEP
const uint8_t PIN_DIR  = 2;   // DIR

/// ---------- NOTAS ----------
enum Nota { DO=0, RE=2, MI=4, FA=5, SOL=7, LA=9, SI=11 };
enum Alteracion { NATURAL=0, SOSTENIDO=+1, BEMOL=-1 };

/// ---------- DURACIONES ----------
enum Figura {
  REDONDA, BLANCA, NEGRA, CORCHEA, SEMICORCHEA, FUSA, SEMIFUSA,
  P_REDONDA, P_BLANCA, P_NEGRA, P_CORCHEA,
  T_NEGRA, T_CORCHEA, T_SEMICORCHEA
};

/// ---------- ESCALAS ----------
enum Escala { MAYOR, MENOR_NATURAL, MENOR_ARMONICA, MENOR_MELODICA };

/// ---------- ESTADO GLOBAL ----------
struct EstadoMusical {
  float bpm = 120.0f;
  int8_t transposicion = 0; // semitonos
  bool direccionAlta = false;
} EM;

/// ---------- FUNCIONES AUXILIARES ----------

// Convierte figura musical a milisegundos
uint16_t duracionMs(Figura f) {
  const float negra = 60000.0f / EM.bpm; // duración de negra en ms
  switch(f) {
    case REDONDA:       return negra * 4;
    case BLANCA:        return negra * 2;
    case NEGRA:         return negra * 1;
    case CORCHEA:       return negra * 0.5;
    case SEMICORCHEA:   return negra * 0.25;
    case FUSA:          return negra * 0.125;
    case SEMIFUSA:      return negra * 0.0625;
    case P_REDONDA:     return negra * 6;
    case P_BLANCA:      return negra * 3;
    case P_NEGRA:       return negra * 1.5;
    case P_CORCHEA:     return negra * 0.75;
    case T_NEGRA:       return (negra * 2) / 3;
    case T_CORCHEA:     return (negra) / 3;
    case T_SEMICORCHEA: return (negra * 0.5) / 3;
  }
  return negra;
}

// MIDI a frecuencia
inline float midiAHercio(int midi) {
  return 440.0f * powf(2.0f, (midi - 69) / 12.0f); // La4=440 Hz
}

// Nota → MIDI
int notaAMidi(Nota n, int octava, Alteracion a = NATURAL) {
  int semitono = (int)n + (int)a;
  int midi = 12 * (octava + 1) + semitono;
  return midi;
}

// Generador de pulsos al motor
void tocarFrecuencia(float hz, uint16_t ms, bool dirAlta) {
  if (hz <= 0.0f || ms == 0) { delay(ms); return; }
  const float periodo_us = 1000000.0f / hz;
  unsigned long medio_us = (unsigned long)(periodo_us * 0.5f);
  unsigned long tFinal = millis() + ms;

  digitalWrite(PIN_DIR, dirAlta ? HIGH : LOW);
  while (millis() < tFinal) {
    digitalWrite(PIN_PASO, HIGH);
    delayMicroseconds(medio_us);
    digitalWrite(PIN_PASO, LOW);
    delayMicroseconds(medio_us);
  }
}

// ---------- APLICACIÓN ----------
void tempo(float bpm)     { EM.bpm = bpm; }
void transponer(int st)   { EM.transposicion = st; }
void direccionDefecto(bool hi) { EM.direccionAlta = hi; }
bool alternarDireccion = false;  // estado global para ir alternando la dirección al que gira el motor

void nota(Nota n, int octava, Figura f, Alteracion a = NATURAL) {
  int midi = notaAMidi(n, octava, a) + EM.transposicion;
  float hz = midiAHercio(midi);
  tocarFrecuencia(hz, duracionMs(f), alternarDireccion);
  alternarDireccion = !alternarDireccion; // cambia el sentido para la próxima nota
}

void silencio(Figura f) {
  tocarFrecuencia(0.0f, duracionMs(f), EM.direccionAlta);
}

void setup() {
  pinMode(PIN_PASO, OUTPUT);
  pinMode(PIN_DIR, OUTPUT);

  tempo(116);
  transponer(0); 
  direccionDefecto(false);
}

void loop() {
  // Barra 1
  nota(FA, 4, CORCHEA);
  nota(LA, 4, CORCHEA, BEMOL);
  nota(FA, 4, CORCHEA);
  nota(DO, 5, CORCHEA);
  nota(FA, 4, CORCHEA);
  nota(LA, 4, CORCHEA, BEMOL);
  nota(FA, 4, CORCHEA);
  nota(DO, 5, CORCHEA);

  // Barra 2 (variante)
  nota(FA, 4, CORCHEA);
  nota(LA, 4, CORCHEA, BEMOL);
  nota(FA, 4, CORCHEA);
  nota(DO, 5, CORCHEA);
  nota(FA, 4, CORCHEA);
  nota(LA, 4, CORCHEA, BEMOL);
  nota(FA, 4, CORCHEA);
  nota(MI, 5, CORCHEA, BEMOL);   // MI♭ (color característico)

  // Repite barras 1–2 a gusto (4x típico)

  nota(MI, 5, CORCHEA, BEMOL);
  nota(FA, 5, CORCHEA);
  nota(SOL,5, CORCHEA);
  nota(LA, 5, CORCHEA, BEMOL);
  nota(SI, 5, CORCHEA, BEMOL);
  nota(LA, 5, CORCHEA, BEMOL);
  nota(SOL,5, CORCHEA);
  nota(FA, 5, CORCHEA);

  nota(MI, 5, CORCHEA, BEMOL);
  nota(FA, 5, CORCHEA);
  nota(SOL,5, CORCHEA);
  nota(LA, 5, CORCHEA, BEMOL);
  nota(SI, 5, CORCHEA, BEMOL);
  nota(LA, 5, CORCHEA, BEMOL);
  nota(SOL,5, CORCHEA);
  nota(DO, 5, CORCHEA);

}
