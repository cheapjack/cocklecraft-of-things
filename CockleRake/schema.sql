drop table if exists cockles;
create table cockles (
  id integer primary key autoincrement,
  first text not null,
  second text not null,
  workshop text not null
);
drop table if exists readings;
create table readings (
  id integer primary key autoincrement,
  cockle_id integer not null,
  temperature float not null
);

