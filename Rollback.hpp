# pragma once
# include <iostream>
# include <deque>

namespace my_lib
{
	template <class T>
	class Rollback
	{
	private:
		std::deque<T> data;

		size_t index;

	public:
		inline Rollback(T t) : index(0)
		{
			data.emplace_back(t);
		}

		inline void set(T t) noexcept
		{
			while (data.size() != index + 1)
				data.pop_back();

			data.emplace_back(t);

			index = data.size() - 1;
		}

		template <class... Args>
		inline void set(Args... args)
		{
			while (data.size() != index + 1)
				data.pop_back();

			data.emplace_back(args...);

			index = data.size() - 1;
		}

		inline T& undo() noexcept
		{
			if (index > 0)
				--index;

			return data.at(index);
		}

		inline T& redo() noexcept
		{
			if (index < data.size() - 1)
				++index;

			return data.at(index);
		}

		inline T& reset() noexcept
		{
			const T tmp = data.front();

			data.clear();
			index = 0;
			data.emplace_back(tmp);

			return data.front();
		}

		inline T& compress() noexcept
		{
			const T tmp_front = data.front();
			const T tmp_back = data.back();

			data.clear();
			index = 1;
			data.emplace_back(tmp_front);
			data.emplace_back(tmp_back);

			return data.back();
		}

		inline T& get() noexcept
		{
			return data.at(index);
		}

		inline const T& get() const noexcept
		{
			return data.at(index);
		}

		inline size_t size() const noexcept 
		{
			return data.size();
		}

		inline bool can_undo() const noexcept 
		{
			return index > 0;
		}

		inline bool can_redo() const noexcept 
		{
			return index < data.size() - 1;
		}

		inline Rollback<T>& operator =(Rollback<T> t) noexcept
		{
			*this = t;

			return *this;
		}

		inline T& operator =(T t) noexcept
		{
			set(t);

			return data.back();
		}

		inline T& operator +=(T t) noexcept
		{
			const T tmp = data.at(index);
			set(tmp + t);

			return data.back();
		}

		inline T& operator -=(T t) noexcept
		{
			const T tmp = data.at(index);
			set(tmp - t);

			return data.back();
		}

		inline T& operator *=(T t) noexcept
		{
			const T tmp = data.at(index);
			set(tmp * t);

			return data.back();
		}

		inline T& operator /=(T t) noexcept
		{
			const T tmp = data.at(index);
			set(tmp / t);

			return data.back();
		}
		
		inline T& operator %=(T t) noexcept
		{
			const T tmp = data.at(index);
			set(tmp / t);

			return data.back();
		}

		inline T& operator ++() noexcept
		{
			const T tmp = data.at(index);
			set(tmp + 1);

			return data.back();
		}

		inline T operator ++(int) noexcept
		{
			const T tmp = data.at(index);
			set(tmp + 1);

			return tmp;
		}

		inline T& operator --() noexcept
		{
			const T tmp = data.at(index);
			set(tmp - 1);

			return data.back();
		}

		inline T operator --(int) noexcept
		{
			const T tmp = data.at(index);
			set(tmp - 1);

			return T;
		}

		inline T operator +(T t) const noexcept
		{
			const T tmp = data.at(index);

			return tmp + t;
		}

		inline T operator -(T t) const noexcept
		{
			const T tmp = data.at(index);

			return tmp - t;
		}

		inline T operator *(T t) const noexcept
		{
			const T tmp = data.at(index);

			return tmp * t;
		}

		inline T operator /(T t) const noexcept
		{
			const T tmp = data.at(index);

			return tmp / t;
		}

		inline T operator %(T t) const noexcept
		{
			const T tmp = data.at(index);

			return tmp % t;
		}

		inline bool operator !() const noexcept
		{
			return !data.at(index);
		}

		inline T& operator <<(size_t n) noexcept
		{
			for (size_t i = 0; i < n; ++i)
				undo();

			return data.at(index);
		}

		inline T& operator >>(size_t n) noexcept
		{
			for (size_t i = 0; i < n; ++i)
				redo();

			return data.at(index);
		}

		inline operator T() const noexcept
		{
			return data.at(index);
		}
	};

	template<class T>
	inline T operator +(const T& lhs, const Rollback<T>& rhs) noexcept
	{
		const T tmp = rhs.get();
		return lhs + tmp;
	}

	template<class T>
	inline T operator -(const T& lhs, const Rollback<T>& rhs) noexcept
	{
		const T tmp = rhs.get();
		return lhs - tmp;
	}

	template<class T>
	inline T operator *(const T& lhs, const Rollback<T>& rhs) noexcept
	{
		const T tmp = rhs.get();
		return lhs * tmp;
	}

	template<class T>
	inline T operator /(const T& lhs, const Rollback<T>& rhs) noexcept
	{
		const T tmp = rhs.get();
		return lhs / tmp;
	}

	template<class T>
	inline T operator %(const T& lhs, const Rollback<T>& rhs) noexcept
	{
		const T tmp = rhs.get();
		return lhs % tmp;
	}

	template<class T>
	std::ostream& operator <<(std::ostream& o, const Rollback<T>& t)
	{
		o << t.get();
		return o;
	}
}