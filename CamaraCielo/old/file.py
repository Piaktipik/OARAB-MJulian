# -*- coding: utf-8 -*-
"""
Created on Wed Aug 22 22:09:27 2018

@author: Piaktipik
"""
file = open("conteo.txt", "r") 
num  = file.read() 
print "conteo:", str(num)
numero = int(num)
numero = numero + 1

file = open("conteo.txt","w") 
file.write(str(numero)) 
 
file.close() 

