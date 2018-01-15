//check for factory default reset jumper on pin ?
#if defined (FACTORY_DEFAULT_PIN)
   pinMode(FACTORY_DEFAULT_PIN, INPUT);
   if (digitalRead(FACTORY_DEFAULT_PIN) != 1) 
	nm.forceInitAll();
 #endif 