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
  created_at text not null,
  cockle_id integer not null,
  temperature real not null
);
drop table if exists observations;
create table observations (
  id integer primary key autoincrement,
  created_at text not null,
  location text not null,
  observation text not null
);

