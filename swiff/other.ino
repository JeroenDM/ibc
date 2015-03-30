/* Random collection of functions, mostly support functionallity */

// Check how much ram is free for variable storage
// Code from http://playground.arduino.cc/code/AvailableMemory
int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void checkRam () {
  Serial.print("Free sRAM: ");
  Serial.print(freeRam());
  Serial.println(" bytes");
}
