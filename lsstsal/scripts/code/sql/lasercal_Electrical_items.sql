DROP TABLE IF EXISTS lasercal_Electrical_items;
CREATE TABLE lasercal_Electrical_items (
  num	int,
  item	char(32),
  type  char(32),
  size  int,
  units char(32),
  freq  float,
  range char(32),
  location char(32),
  comment char(128),
  PRIMARY KEY (num)
);
INSERT INTO lasercal_Electrical_items VALUES (1,"Raw","long",16,"",0.054,"","","");
INSERT INTO lasercal_Electrical_items VALUES (2,"Calibrated","float",32,"",0.054,"","","");
