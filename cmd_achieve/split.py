#!/usr/bin/env python
# coding=utf-8
first = 0
second = 1
while first < 100:
    print first
    first, second = second, first + second
print 'Everything is done'

