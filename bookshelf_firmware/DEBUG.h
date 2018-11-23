

// some serial debug macros that can be easily toggled on/off
#ifdef DEBUG
 #define DEBUG_PRINTLN(x)  Serial.println (x)
 #define DEBUG_PRINT(x)  Serial.print (x)
 #define DEBUG_WRITE(x)  Serial.write (x)
#else
 #define DEBUG_PRINTLN(x)
 #define DEBUG_PRINT(x)
 #define DEBUG_WRITE(x)
#endif


