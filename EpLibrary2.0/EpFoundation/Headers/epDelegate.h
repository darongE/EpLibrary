/*! 
@file epDelegate.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
@date March 08, 2012
@brief A C# Style Delegate Interface
@version 2.0

@section LICENSE

Copyright (C) 2012  Woong Gyu La <juhgiyo@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

@section DESCRIPTION

An Interface for C# Style Delegate Class.

*/
#ifndef __EP_DELEGATE_H__
#define __EP_DELEGATE_H__
#include "epFoundationLib.h"
#include "epSystem.h"
#include <vector>
#include "epCriticalSectionEx.h"
#include "epMutex.h"
#include "epNoLock.h"

using namespace std;

namespace epl
{
	/*! 
	@class Delegate epDelegate.h
	@brief A class for C# Style Delegate.
	*/
	template<typename RetType,typename ArgType>
	class Delegate{
	public:
		/// Function Pointer Type Definition
		typedef RetType (*FuncType) (ArgType);

		/*!
		Default Constructor

		Initializes the delegate
		@param[in] lockPolicyType The lock policy
		*/
		Delegate(LockPolicy lockPolicyType=EP_LOCK_POLICY)
		{
			m_lockPolicy=lockPolicyType;
			switch(lockPolicyType)
			{
			case LOCK_POLICY_CRITICALSECTION:
				m_lock=EP_NEW CriticalSectionEx();
				break;
			case LOCK_POLICY_MUTEX:
				m_lock=EP_NEW Mutex();
				break;
			case LOCK_POLICY_NONE:
				m_lock=EP_NEW NoLock();
				break;
			default:
				m_lock=NULL;
				break;
			}
		}

		/*!
		Default Constructor

		Initializes the delegate with given function pointer
		@param[in] func the initial function pointer
		@param[in] lockPolicyType The lock policy
		*/
		Delegate(RetType (*func)(ArgType),LockPolicy lockPolicyType=EP_LOCK_POLICY)
		{
			m_lockPolicy=lockPolicyType;
			m_funcList.push_back(func);
			switch(lockPolicyType)
			{
			case LOCK_POLICY_CRITICALSECTION:
				m_lock=EP_NEW CriticalSectionEx();
				break;
			case LOCK_POLICY_MUTEX:
				m_lock=EP_NEW Mutex();
				break;
			case LOCK_POLICY_NONE:
				m_lock=EP_NEW NoLock();
				break;
			default:
				m_lock=NULL;
				break;
			}
		}

		/*!
		Copy Constructor

		Initializes the delegate with given delegate
		@param[in] orig the delegate
		*/
		Delegate(const Delegate<RetType,ArgType> &orig)
		{
			m_lockPolicy=orig.m_lockPolicy;
			m_funcList=orig.m_funcList;
			switch(m_lockPolicy)
			{
			case LOCK_POLICY_CRITICALSECTION:
				m_lock=EP_NEW CriticalSectionEx();
				break;
			case LOCK_POLICY_MUTEX:
				m_lock=EP_NEW Mutex();
				break;
			case LOCK_POLICY_NONE:
				m_lock=EP_NEW NoLock();
				break;
			default:
				m_lock=NULL;
				break;
			}
		}

		/*!
		Default Destructor

		Destroy the delegate
		*/
		virtual ~Delegate()
		{
			if(m_lock)
				EP_DELETE m_lock;
		}	

		/*!
		Assignment Operator Overloading

		the Packet set as given packet b
		@param[in] b right side of packet
		@return this object
		*/
		Delegate<RetType,ArgType> & operator=(const Delegate<RetType,ArgType>&b)
		{
			if(this!=&b)
			{
				LockObj lock(m_lock);
				m_funcList=b.m_funcList;
			}
			return *this;
		}

		/*!
		Initialize the delegate with given function pointer
		@param[in] func The initial function pointer
		@return reference to this delegate
		*/
		virtual Delegate<RetType,ArgType> & operator =(RetType (*func)(ArgType))
		{
			LockObj lock(m_lock);
			m_funcList.clear();
			m_funcList.push_back(func);
			return *this;
		}

		/*!
		Append the delegate with given function pointer
		@param[in] func The function pointer to append
		@return reference to this delegate
		*/
		virtual Delegate<RetType,ArgType> & operator +=(RetType (*func)(ArgType))
		{
			LockObj lock(m_lock);
			m_funcList.push_back(func);
			return *this;
		}

		/*!
		Add the delegate with given function pointer
		@param[in] func The function pointer to append
		@return the delegate with the function pointers from this delegate and given function pointer
		*/
		virtual Delegate<RetType,ArgType> operator +(RetType (*func)(ArgType)) const
		{
			Delegate<RetType,ArgType> ret(*this);
			ret+=func;
			return ret;
		}

		/*!
		Append the delegate with function pointers from the given delegate
		@param[in] right The delegate to append
		@return reference to this delegate
		*/
		virtual Delegate<RetType,ArgType> & operator +=(const Delegate<RetType,ArgType> &right)
		{
			LockObj lock(m_lock);
			vector<RetType (*)(ArgType)>::iterator iter;
			for(iter=right.m_funcList.begin();iter!=right.m_funcList.end();iter++)
			{
				m_funcList.push_back(iter);
			}
			return *this;
		}

		/*!
		Add the delegate with function pointers from the given delegate
		@param[in] right The delegate to append
		@return the delegate with the function pointers from this delegate and given delegate
		*/
		virtual Delegate<RetType,ArgType> operator +(const Delegate<RetType,ArgType> &right) const
		{
			Delegate<RetType,ArgType> ret(*this);
			ret+=right;
			return ret;
		}

		/*!
		Remove the given function pointers from this delegate
		@param[in] func The function pointer to remove
		@return reference to this delegate
		*/
		virtual Delegate<RetType,ArgType> & operator -=(RetType (*func)(ArgType))
		{
			LockObj lock(m_lock);
			vector<RetType (*)(ArgType)>::iterator iter;
			for(iter=m_funcList.begin();iter!=m_funcList.end();)
			{
				if(*iter==func)
				{
					iter=m_funcList.erase(iter);
				}
				else 
					iter++;
			}

			return *this;
		}

		/*!
		Remove given function pointer from this delegate
		@param[in] func The function pointer to remove
		@return the delegate with given function pointer extracted from this delegate
		*/
		virtual Delegate<RetType,ArgType> operator -(RetType (*func)(ArgType)) const
		{
			Delegate<RetType,ArgType> ret(*this);
			ret-=func;
			return ret;
		}

		/*!
		Remove the function pointers of the given delegate from this delegate
		@param[in] right The delegate to remove
		@return reference to this delegate
		*/
		virtual Delegate<RetType,ArgType> & operator -=(const Delegate<RetType,ArgType> &right)
		{
			LockObj lock(m_lock);
			vector<RetType (*)(ArgType)>::iterator rightIter;
			vector<RetType (*)(ArgType)>::iterator iter;
			for(rightIter=right.m_funcList.begin();rightIter!=right.m_funcList.end();rightIter++)
			{
				for(iter=m_funcList.begin();iter!=m_funcList.end();)
				{
					if(*iter==*rightIter)
					{
						m_funcList.erase(iter);
					}
					else 
						iter++;
				}
			}
			return *this;
		}

		/*!
		Remove the function pointers of the given delegate from this delegate 
		@param[in] right The delegate to remove
		@return the delegate with the function pointers of given delegate extracted from this delegate
		*/
		virtual Delegate<RetType,ArgType> operator -(const Delegate<RetType,ArgType> &right) const
		{
			Delegate<RetType,ArgType> ret(*this);
			ret-=right;
			return ret;
		}

		/*!
		Return the function pointer at given index
		@param[in] idx the index to return the function pointer
		@return the function pointer at given index
		*/
		virtual FuncType operator [](unsigned int idx) const
		{
			EP_VERIFY_OUT_OF_RANGE(idx<m_funcList.size());
			vector<RetType (*)(ArgType)>::iterator iter=m_funcList.begin();
			iter+=idx;
			return *iter;
		
		}

		/*!
		Execute the function pointers within this delegate with the argument given
		@param[in] arg the argument of the function pointer
		@return the return value of the last function pointer
		*/
		virtual RetType operator ()(ArgType arg)
		{
			vector<RetType (*)(ArgType)>::iterator iter;
			vector<RetType (*)(ArgType)>::iterator lastIterCheck;
			for(iter=m_funcList.begin();iter!=m_funcList.end();iter++)
			{
				lastIterCheck=iter;
				lastIterCheck++;
				if(lastIterCheck==m_funcList.end())
					break;
				else
					(*iter)(arg);
			}
			return (*iter)(arg);
		}

	private:
		/// function pointer list
		vector<RetType (*)(ArgType)> m_funcList;
		/// lock
		BaseLock *m_lock;
		/// Lock Policy
		LockPolicy m_lockPolicy;
	};

	/*! 
	@class Delegate epDelegate.h
	@brief A partial specialization class for C# Style Delegate with void argument.
	*/
	template<typename RetType>
	class Delegate<RetType,void>{
	public:
		/// Function Pointer Type Definition
		typedef RetType (*FuncType) (void);

		/*!
		Default Constructor

		Initializes the delegate
		@param[in] lockPolicyType The lock policy
		*/
		Delegate(LockPolicy lockPolicyType=EP_LOCK_POLICY)
		{
			m_lockPolicy=lockPolicyType;
			switch(lockPolicyType)
			{
			case LOCK_POLICY_CRITICALSECTION:
				m_lock=EP_NEW CriticalSectionEx();
				break;
			case LOCK_POLICY_MUTEX:
				m_lock=EP_NEW Mutex();
				break;
			case LOCK_POLICY_NONE:
				m_lock=EP_NEW NoLock();
				break;
			default:
				m_lock=NULL;
				break;
			}
		}

		/*!
		Default Constructor

		Initializes the delegate with given function pointer
		@param[in] func the initial function pointer
		@param[in] lockPolicyType The lock policy
		*/
		Delegate(RetType (*func)(void),LockPolicy lockPolicyType=EP_LOCK_POLICY)
		{
			m_lockPolicy=lockPolicyType;
			m_funcList.push_back(func);
			switch(lockPolicyType)
			{
			case LOCK_POLICY_CRITICALSECTION:
				m_lock=EP_NEW CriticalSectionEx();
				break;
			case LOCK_POLICY_MUTEX:
				m_lock=EP_NEW Mutex();
				break;
			case LOCK_POLICY_NONE:
				m_lock=EP_NEW NoLock();
				break;
			default:
				m_lock=NULL;
				break;
			}
		}

		/*!
		Copy Constructor

		Initializes the delegate with given delegate
		@param[in] orig the delegate
		*/
		Delegate(const Delegate<RetType,void> &orig)
		{
			m_lockPolicy=orig.m_lockPolicy;
			m_funcList=orig.m_funcList;
			switch(m_lockPolicy)
			{
			case LOCK_POLICY_CRITICALSECTION:
				m_lock=EP_NEW CriticalSectionEx();
				break;
			case LOCK_POLICY_MUTEX:
				m_lock=EP_NEW Mutex();
				break;
			case LOCK_POLICY_NONE:
				m_lock=EP_NEW NoLock();
				break;
			default:
				m_lock=NULL;
				break;
			}
		}

		/*!
		Default Destructor

		Destroy the delegate
		*/
		virtual ~Delegate()
		{
			if(m_lock)
				EP_DELETE m_lock;
		}

		/*!
		Assignment Operator Overloading

		the Delegate set as given packet b
		@param[in] b right side of packet
		@return this object
		*/
		Delegate<RetType,void> & operator=(const Delegate<RetType,void>&b)
		{
			if(this!=&b)
			{
				LockObj lock(m_lock);
				m_funcList=b.m_funcList;
			}
			return *this;
		}

		/*!
		Initialize the delegate with given function pointer
		@param[in] func The initial function pointer
		@return reference to this delegate
		*/
		virtual Delegate<RetType,void> & operator =(RetType (*func)(void))
		{
			LockObj lock(m_lock);
			m_funcList.clear();
			m_funcList.push_back(func);
			return *this;
		}

		/*!
		Append the delegate with given function pointer
		@param[in] func The function pointer to append
		@return reference to this delegate
		*/
		virtual Delegate<RetType,void> & operator +=(RetType (*func)(void))
		{
			LockObj lock(m_lock);
			m_funcList.push_back(func);
			return *this;
		}

		/*!
		Add the delegate with given function pointer
		@param[in] func The function pointer to append
		@return the delegate with the function pointers from this delegate and given function pointer
		*/
		virtual Delegate<RetType,void> operator +(RetType (*func)(void)) const
		{
			Delegate<RetType,void> ret(*this);
			ret+=func;
			return ret;
		}

		/*!
		Append the delegate with function pointers from the given delegate
		@param[in] right The delegate to append
		@return reference to this delegate
		*/
		virtual Delegate<RetType,void> & operator +=(const Delegate<RetType,void> &right)
		{
			LockObj lock(m_lock);
			vector<RetType (*)(void)>::iterator iter;
			for(iter=right.m_funcList.begin();iter!=right.m_funcList.end();iter++)
			{
				m_funcList.push_back(iter);
			}
			return *this;
		}

		/*!
		Add the delegate with function pointers from the given delegate
		@param[in] right The delegate to append
		@return the delegate with the function pointers from this delegate and given delegate
		*/
		virtual Delegate<RetType,void> operator +(const Delegate<RetType,void> &right) const
		{
			Delegate<RetType,void> ret(*this);
			ret+=right;
			return ret;
		}

		/*!
		Remove the given function pointers from this delegate
		@param[in] func The function pointer to remove
		@return reference to this delegate
		*/
		virtual Delegate<RetType,void> & operator -=(RetType (*func)(void))
		{
			LockObj lock(m_lock);
			vector<RetType (*)(void)>::iterator iter;
			for(iter=m_funcList.begin();iter!=m_funcList.end();)
			{
				if(*iter==func)
				{
					m_funcList.erase(iter);
				}
				else 
					iter++;
			}

			return *this;
		}

		/*!
		Remove given function pointer from this delegate
		@param[in] func The function pointer to remove
		@return the delegate with given function pointer extracted from this delegate
		*/
		virtual Delegate<RetType,void> operator -(RetType (*func)(void)) const
		{
			Delegate<RetType,void> ret(*this);
			ret-=func;
			return ret;
		}

		/*!
		Remove the function pointers of the given delegate from this delegate
		@param[in] right The delegate to remove
		@return reference to this delegate
		*/
		virtual Delegate<RetType,void> & operator -=(const Delegate<RetType,void> &right)
		{
			LockObj lock(m_lock);
			vector<RetType (*)(void)>::iterator rightIter;
			vector<RetType (*)(void)>::iterator iter;
			for(rightIter=right.m_funcList.begin();rightIter!=right.m_funcList.end();rightIter++)
			{
				for(iter=m_funcList.begin();iter!=m_funcList.end();)
				{
					if(*iter==*rightIter)
					{
						m_funcList.erase(iter);
					}
					else 
						iter++;
				}
			}
			return *this;
		}

		/*!
		Remove the function pointers of the given delegate from this delegate 
		@param[in] right The delegate to remove
		@return the delegate with the function pointers of given delegate extracted from this delegate
		*/
		virtual Delegate<RetType,void> operator -(const Delegate<RetType,void> &right) const
		{
			Delegate<RetType,void> ret(*this);
			ret-=right;
			return ret;
		}

		/*!
		Return the function pointer at given index
		@param[in] idx the index to return the function pointer
		@return the function pointer at given index
		*/
		virtual FuncType operator [](unsigned int idx) const
		{
			EP_VERIFY_OUT_OF_RANGE(idx<m_funcList.size());
			vector<RetType (*)(void)>::iterator iter=m_funcList.begin();
			iter+=idx;
			return *iter;

		}

		/*!
		Execute the function pointers within this delegate
		@return the return value of the last function pointer
		*/
		virtual RetType operator ()()
		{
			vector<RetType (*)(void)>::iterator iter;
			vector<RetType (*)(void)>::iterator lastIterCheck;
			for(iter=m_funcList.begin();iter!=m_funcList.end();iter++)
			{
				lastIterCheck=iter;
				lastIterCheck++;
				if(lastIterCheck==m_funcList.end())
					return (*iter)();
				else
					(*iter)();
			}
		}

	private:
		/// function pointer list
		vector<RetType (*)(void)> m_funcList;
		/// lock
		BaseLock *m_lock;
		/// Lock Policy
		LockPolicy m_lockPolicy;
	};
}
template<typename RetType,typename ArgType>
Delegate<RetType,ArgType> operator +(RetType (*func)(ArgType),const Delegate<RetType,ArgType>& right)
{
	Delegate<RetType,ArgType> ret(func);
	ret+=right;
	return ret;
}

template<typename RetType,typename ArgType>
Delegate<RetType,ArgType> operator +(RetType (*func)(ArgType),RetType (*func2)(ArgType))
{
	Delegate<RetType,ArgType> ret(func);
	ret+=func2;
	return ret;
}

#endif //__EP_DELEGATE_H__