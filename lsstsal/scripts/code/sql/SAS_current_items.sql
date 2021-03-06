DROP TABLE IF EXISTS SAS_current_items;
CREATE TABLE SAS_current_items (
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
INSERT INTO SAS_current_items VALUES (1,"Board_current","float",4,"Volt",0.054,"","");
INSERT INTO SAS_current_items VALUES (2,"REB_ID"," unsignedshort",1,"",0.054,"","");
