DROP TABLE IF EXISTS GAS_checksum_items;
CREATE TABLE GAS_checksum_items (
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
INSERT INTO GAS_checksum_items VALUES (1,"FPGACheckSum","long",2,"none",0.054,"","");
INSERT INTO GAS_checksum_items VALUES (2,"REB_ID"," unsignedshort",1,"",0.054,"","");
