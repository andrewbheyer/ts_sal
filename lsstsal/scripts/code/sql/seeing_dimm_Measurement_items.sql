DROP TABLE IF EXISTS seeing_dimm_Measurement_items;
CREATE TABLE seeing_dimm_Measurement_items (
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
INSERT INTO seeing_dimm_Measurement_items VALUES (1,"hrnum","long",1,"",0.1,"","","");
INSERT INTO seeing_dimm_Measurement_items VALUES (2,"exposure","float",1,"",0.1,"","","");
INSERT INTO seeing_dimm_Measurement_items VALUES (3,"numlines","long",1,"",0.1,"","","");
INSERT INTO seeing_dimm_Measurement_items VALUES (4,"seeing_corr","float",1,"",0.1,"","","");
INSERT INTO seeing_dimm_Measurement_items VALUES (5,"flux","long",1,"",0.1,"","","");
INSERT INTO seeing_dimm_Measurement_items VALUES (6,"scintleft","float",1,"",0.1,"","","");
INSERT INTO seeing_dimm_Measurement_items VALUES (7,"scintright","float",1,"",0.1,"","","");
INSERT INTO seeing_dimm_Measurement_items VALUES (8,"strehlleft","float",1,"",0.1,"","","");
INSERT INTO seeing_dimm_Measurement_items VALUES (9,"strehlright","float",1,"",0.1,"","","");
