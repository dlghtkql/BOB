#-*- coding: utf-8 -*-
import os

path = raw_input("input path:")			# 경로를 입력받습니다.
dir1 = list(os.listdir(path))			# 디렉토리 안에 있는 파일들의 리스틀 받고 정렬합니다.

for files in dir1:
	print files
