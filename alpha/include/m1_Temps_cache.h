typedef struct m1_Temps_cache {
  int cppDummy;
  int syncI;
  long Raw[16];
  float Calibrated[16];
  long Health[2];
  int syncO;
} m1_Temps_cache;
