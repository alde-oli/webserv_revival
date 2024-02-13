#pragma once

#include "libs.hpp"

template <typename T>
//class to autodelete ptr when out of scope
class AutoPtr
{
	public:
		AutoPtr() = delete;
		explicit AutoPtr(T* p = nullptr);
		AutoPtr(const AutoPtr& a) = delete;
		~AutoPtr(); 

		AutoPtr&	operator=(const AutoPtr& a) = delete;
		T&			operator*() const;
		T*			operator->() const;

	private:
		T*			_ptr;
};

template <typename T>
//class to autodelete ptr[] when out of scope
class AutoAPtr
{
	public:
		AutoAPtr() = delete;
		explicit AutoAPtr(T* p = nullptr);
		AutoAPtr(const AutoAPtr& a) = delete;
		~AutoAPtr();

		AutoAPtr&	operator=(const AutoAPtr& a) = delete;
		
		t*			get() const; 

	private:
		T*			_ptr;
};

//class to auto close file descriptor when out of scope
class AutoFD
{
	public:
		AutoFD();
		explicit AutoFD(int fd);
		AutoFD(const AutoFD& a) = delete;
		~AutoFD();

		AutoFD&			operator=(const AutoFD& a) = delete;

		void	set(int fd);
		
		int				get() const;
		void			close();

	private:
		int				_fd;
};
