/*
* Copyright 2017, OYMotion Inc.
* All rights reserved.
*
* IMPORTANT: Your use of this Software is limited to those specific rights
* granted under the terms of a software license agreement between you and
* OYMotion.  You may not use this Software unless you agree to abide by the
* terms of the License. The License limits your use, and you acknowledge,
* that the Software may not be modified, copied or distributed unless used
* solely and exclusively in conjunction with an OYMotion product.  Other
* than for the foregoing purpose, you may not use, reproduce, copy, prepare
* derivative works of, modify, distribute, perform, display or sell this
* Software and/or its documentation for any purpose.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
* THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
* DAMAGE.
*
*/
/*!
* \file utils/expose.h
* \brief 
*
* \version 0.1
* \date 2017.4.20
*/
#pragma once
/// The following ifdef block is the standard way of creating macros which make exporting 
/// from a DLL simpler. All files within this DLL are compiled with the GFORCESDK_EXPORTS
/// symbol defined on the command line. This symbol should not be defined on any project
/// that uses this DLL. This way any other project whose source files include this file see 
/// GFORCESDK_API functions as being imported from a DLL, whereas this DLL sees symbols
/// defined with this macro as being exported.

#ifdef WIN32

#ifdef GFORCESDK_EXPORTS
#define GFORCESDK_API __declspec(dllexport)
#else
#define GFORCESDK_API __declspec(dllimport)
#endif

#else // #ifdef WIN32

#define GFORCESDK_API

#endif
