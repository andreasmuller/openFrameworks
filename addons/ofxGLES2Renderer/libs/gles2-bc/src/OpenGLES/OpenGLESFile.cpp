/*
 Copyright 2009 Johannes Vuorinen
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at 
 
 http://www.apache.org/licenses/LICENSE-2.0 
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */


#include "OpenGLESFile.h"

#ifdef __ANDROID__

#include "OpenGLESUtil.h"

#include <stdio.h>

using namespace OpenGLES;

std::string OpenGLESFile::base_path;

OpenGLESFile::OpenGLESFile(std::string n) : name()
{
	name = (base_path+n).c_str();
	LOG_MESSAGE("OpenGLESFile: " + name);
}

bool OpenGLESFile::open() 
{
	fp = fopen(name.c_str(), "r");
	return fp;
}

char * OpenGLESFile::gets(char * buf, int size)
{
	return fgets(buf, size, fp);
}

int OpenGLESFile::seek(long int offset, int origin)
{
	return fseek(fp, offset, origin);
}

long int OpenGLESFile::tell()
{
	return ftell(fp);
}

size_t OpenGLESFile::read(void *ptr, size_t size, size_t count)
{
	return fread(ptr, size, count, fp);
}

void OpenGLESFile::close()
{
	fclose(fp);
}

std::string OpenGLESFile::getName()
{
	return name;
}

void OpenGLESFile::setBasePath(std::string path){
	base_path = path;
}

#endif
