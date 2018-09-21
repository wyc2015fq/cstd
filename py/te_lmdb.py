#!/usr/bin/env python

import lmdb
import os, sys

def str2byte(x):
	# print(type(x))
	if isinstance(x, int):
		return str(x).encode('utf-8')

	if isinstance(x, str):
		return x.encode('utf-8')

	return "?";

def initialize(name):
    env = lmdb.open(name);
    return env;

def insert(env, sid, name):
    txn = env.begin(write = True);
    txn.put(str2byte(sid), str2byte(name));
    txn.commit();

def delete(env, sid):
    txn = env.begin(write = True);
    txn.delete(str2byte(sid));
    txn.commit();

def update(env, sid, name):
    txn = env.begin(write = True);
    txn.put(str2byte(sid), str2byte(name));
    txn.commit();

def search(env, sid):
    txn = env.begin();
    name = txn.get(str2byte(sid));
    return name;

def display(env):
    txn = env.begin();
    cur = txn.cursor();
    for key, value in cur:
        print(key, value);

env = initialize("students");

print("Insert 3 records.")
insert(env, "1", "Alice");
insert(env, 2, "Bob");
insert(env, 3, "Peter");
display(env);

print("Delete the record where sid = 1.")
delete(env, 1);
display(env);

print("Update the record where sid = 3.")
update(env, 3, "Mark");
display(env);

print("Get the name of student whose sid = 3.")
name = search(env, 3);
print(name);

name = search(env, 30);
print(name);

env.close();

os.system("rm -r students");
