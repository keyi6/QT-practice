# installation
## sql
you can set `password` and `user` in `server.js`
```sql
create database im;

create table user ( 
		username varchar(100), 
		password char(32), 
		ip varchar(20), 
		id int primary key auto_increment
);

create table record (
		sender int, 
		receiver int, 
		msg varchar(1000), 
		id int primary key auto_increment
);
```
