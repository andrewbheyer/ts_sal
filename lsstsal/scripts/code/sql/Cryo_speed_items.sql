DROP TABLE IF EXISTS Cryo_speed_items;
CREATE TABLE Cryo_speed_items (
  num	int,
  item	char(32),
  type  char(32),
  size  int,
  units char(32),
  freq  float,
  range char(32),
  comment char(128),
  PRIMARY KEY (num)
);
INSERT INTO Cryo_speed_items VALUES (1,"Compressor_speed","float",2,"Hz",0.054,"","");
INSERT INTO Cryo_speed_items VALUES (2,"Dischaarge_pressure","float",2,"Pa",0.054,"","");
INSERT INTO Cryo_speed_items VALUES (3,"Discharge_temp","float",2,"Celsius",0.054,"","");
