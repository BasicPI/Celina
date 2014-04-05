/*****************************************************************************
 * HAL.h
 *
 * Contain the base class used for all other HAL modules. The primary
 * function is to provide a "IsAvailable" function and logical mapping.
 *
 * IsAvailable return true with exception of the DummyModule that is used
 * for devices that is not available.
 *
 * License
 *
 * Copyright (c) 2012, Jan Berger. All rights reserved.
 * email:janvb@live.com
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *	  this list of conditions and the following disclaimer.
 *
 *  * Neither the name of the authors nor the names of its contributors
 *	  may be used to endorse or promote products derived from this software
 *	  without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ****************************************************************************/
#ifndef _HAL
#define _HAL

namespace hal
{
	/*************************************************************************
	 * HAL
	 *
	 * Base class for all HAL modules to provide common features needed by
	 * all.
	 ************************************************************************/
	class HAL
	{
	public:
		HAL()
		{
		}

		virtual bool IsAvailable()
		{
			return true;
		}

	};

	/*************************************************************************
	 * DummyModule
	 *
	 * DummyModule is used to replace modules that are not precent for selected
	 * devices. The main purpose ir to return false on IsAvailable.
	 *
	 * Dummy that return false on IsAvailable. Used to reprecent modules that
	 * are not available so the system can detect hardware precens real-time.
	 ************************************************************************/
	class DummyModule : public HAL
	{
	public:
		DummyModule()
		{

		}

		virtual bool IsAvailable()
		{
			return false;
		}
	};

}

#endif
