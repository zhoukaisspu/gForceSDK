#!/usr/bin/env python3
#-*- coding: utf-8 -*-
#
# Copyright 2017, OYMotion Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
# THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
# DAMAGE.
#
#

import os
import sys
import re
import time

license = (
	'Copyright COPYYEAR, OYMotion Inc.',
	'All rights reserved.',
	'',
	'Redistribution and use in source and binary forms, with or without',
	'modification, are permitted provided that the following conditions',
	'are met:',
	'',
	'1. Redistributions of source code must retain the above copyright',
	'   notice, this list of conditions and the following disclaimer.',
	'',
	'2. Redistributions in binary form must reproduce the above copyright',
	'   notice, this list of conditions and the following disclaimer in',
	'   the documentation and/or other materials provided with the',
	'   distribution.',
	'',
	'THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS',
	'"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT',
	'LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS',
	'FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE',
	'COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,',
	'INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,',
	'BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS',
	'OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED',
	'AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,',
	'OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF',
	'THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH',
	'DAMAGE.',
	)

old_license = (
	'Copyright COPYYEAR, OYMotion Inc.',
	'All rights reserved.',
	'',
	'IMPORTANT: Your use of this Software is limited to those specific rights',
	'granted under the terms of a software license agreement between you and',
	'OYMotion.  You may not use this Software unless you agree to abide by the',
	'terms of the License. The License limits your use, and you acknowledge,',
	'that the Software may not be modified, copied or distributed unless used',
	'solely and exclusively in conjunction with an OYMotion product.  Other',
	'than for the foregoing purpose, you may not use, reproduce, copy, prepare',
	'derivative works of, modify, distribute, perform, display or sell this',
	'Software and/or its documentation for any purpose.',
	'',
	'THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS',
	'"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT',
	'LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS',
	'FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE',
	'COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,',
	'INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,',
	'BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS',
	'OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED',
	'AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,',
	'OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF',
	'THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH',
	'DAMAGE.',
	)

apply_command = 'apply'
year_string = 'COPYYEAR'
cpp_like_file_ext = ('.c', '.cpp', '.cxx', '.h', '.hpp', '.hxx', '.cs', '.java')
script_like_file_ext = ('.py', '.mk')

###############################################
def add_license_cpp(file, license_text, start_pos = -1):
	file.seek(0)
	# pass the leading lines if those are special comments
	lines = file.readlines()
	if not lines:
		lines = []
	insert_pos = 0
	
	license = []
	if start_pos == -1:
		lines.insert(insert_pos, '/*\n')
		insert_pos += 1
	else:
		insert_pos = start_pos
		
	for i in range(len(license_text)):
		if i == 0:
			year = '{}'.format(time.localtime(time.time()).tm_year)
			lines.insert(insert_pos, ' * ' + license_text[i].replace(year_string, year) + '\n')
		else:
			lines.insert(insert_pos, ' * ' + license_text[i] + '\n')
		insert_pos += 1
	if start_pos == -1:
		lines.insert(insert_pos, ' *\n */\n')
	file.seek(0)
	s = ''.join(lines)
	file.write(s)
	file.truncate()
	file.flush()

def remove_license_cpp(file, license_text, lines_range):
	remove_license_script(file, license_text, lines_range)
	
###############################################
def add_license_script(file, license_text, start_pos = -1):
	file.seek(0)
	# pass the leading lines if those are special comments
	lines = file.readlines()
	if not lines:
		lines = []
	insert_pos = 0
	if start_pos == -1:
		special_pat1 = '-*- coding:'
		if len(lines) >= 1:
			if lines[0].find('#!') != -1 or lines[0].find(special_pat1) != -1:
				insert_pos = 1
		if insert_pos == 1 and len(lines) >= 2:
			if lines[1].find(special_pat1) != -1:
				insert_pos = 2
	else:
		insert_pos = start_pos
	
	license = []
	for i in range(len(license_text)):
		if i == 0:
			year = '{}'.format(time.localtime(time.time()).tm_year)
			lines.insert(insert_pos, '# ' + license_text[i].replace(year_string, year) + '\n')
		else:
			lines.insert(insert_pos, '# ' + license_text[i] + '\n')
		insert_pos += 1
	file.seek(0)
	s = ''.join(lines)
	file.write(s)
	file.truncate()
	file.flush()
	
def remove_license_script(file, license_text, lines_range):
	file.seek(0)
	# pass the leading lines if those are special comments
	lines = file.readlines()
	if not lines:
		return
	if len(lines) < lines_range[1]:
		return
	del lines[lines_range[0]:lines_range[1]+1]
	file.seek(0)
	s = ''.join(lines)
	file.write(s)
	file.truncate()
	file.flush()
	
###############################################
def check_license_exist(file, license_text):
	file.seek(0)
	# replace line 0 to re text
	line_start = -1
	line_end = -1
	test_line0 = license_text[0].replace(year_string, r'\d{2,4}')
	re_text_line0 = re.compile(test_line0)
	#print(re_text_line0)
	linecount = 0
	while True:
		line = file.readline()
		if not line:
			break
		linecount += 1
		if line_start == -1:
			line0_found = re_text_line0.search(line)
			if not line0_found:
				continue
			line_start = linecount
			line_match_count = 1
			continue
		position = line.lower().find(license_text[line_match_count].lower().strip())
		if position == -1:
			# not match, restart
			line_start = -1
			line_match_count = 0
			continue
		line_match_count += 1
		if line_match_count >= len(license_text):
			line_end = linecount
			break
			
	#print(file, line_start, line_end)
	if line_start != -1 and line_end != -1:
		return (line_start-1, line_end-1)
	else:
		return False

###############################################
def apply_license_to_file(filename, displayonly):
	old_lic_exist = False
	new_lic_exist = False
	extname = os.path.splitext(filename)[1]
	if extname in cpp_like_file_ext:
		cpplike = True
	elif extname in script_like_file_ext:
		cpplike = False
	else:
		return
	try:
		with open(filename, 'r+') as f:
			old_lic_exist = check_license_exist(f, old_license)
			new_lic_exist = check_license_exist(f, license)
				
			if not old_lic_exist and new_lic_exist:
				return
				
			if displayonly:
				print(filename)
				return
				
			if old_lic_exist:
				print('remove old license', filename)
				if cpplike:
					remove_license_cpp(f, old_license, old_lic_exist)
				else:
					remove_license_script(f, old_license, old_lic_exist)
			if not new_lic_exist:
				print('insert license', filename)
				if cpplike:
					add_license_cpp(f, license, old_lic_exist[0] if old_lic_exist else -1)
				else:
					add_license_script(f, license, old_lic_exist[0] if old_lic_exist else -1)
	except IOError as e:
		print('failed to operate', filename, e)
	
def apply_license_to_folder(folder, displayonly):
	print('Apply license: searching path: ', folder)
	for dirpath, dirnames, filenames in os.walk(folder):
		for file in filenames:
			apply_license_to_file(os.path.join(dirpath, file), displayonly)

def show_help(command):
	help = '''
1. apply license to all source files:
\t%s %s [path]

2. show files can be applied:
\t%s [path]

3. show this help:
\t%s
'''
	_cmd = os.path.basename(command)
	print(help % (_cmd, apply_command, _cmd, _cmd))
	print(old_license)
	return
 
 
if __name__ == "__main__":
	pathname = ''
	displayonly = True
	if len(sys.argv) > 2 and sys.argv[1].lower() == apply_command.lower():
		pathname = sys.argv[2]
		displayonly = False
	elif len(sys.argv) == 2:
		pathname = sys.argv[1]

	if os.path.isfile(pathname):
		apply_license_to_file(pathname, displayonly)
	elif os.path.isdir(pathname):
		apply_license_to_folder(pathname, displayonly)
	else:
		show_help(sys.argv[0])

