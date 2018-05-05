#!/bin/bash
#DataBase Configuration

echo "Start to configure database for Roy's Project!"
if type mysql >/dev/null 2>&1; then
	echo "MySQL installed!, Continue.."
else
	echo "Please install MySQL first, setup and run this file again!"
	sudo apt-get install mysql-server
	exit
fi
echo "Please enter your username:"
read username
echo "Please enter your password:"
read pass
echo $username $pass
if [ -z "$pass" ]&&[ -z "$username"]; then
	pass=258258
	username=root
	echo "password and username set as default!"
fi
export username pass
mysql -u $username -p$pass << EOF
use mysql;
CREATE DATABASE IF NOT EXISTS testdb;
use testdb;
CREATE TABLE IF NOT EXISTS testdata (
	bsdID VARCHAR(20) NOT NULL,
	time DATETIME,
	alarmSystem INT,
	waterDetect INT
);
\q
EOF
echo "Configure done, check if threre are some errors.. running file!"
if [ ! -f server.py ]; then
	echo "File not found!"
else
	chmod +x server.py
	python server.py
fi

