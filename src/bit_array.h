#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H
#include <vector>
#include <algorithm>
#ifdef QT_CORE_LIB
#include <QByteArray>
#endif

struct bit_array {
	using storage_t = std::vector<int8_t>;
	using value_type = storage_t::value_type;
	using size_type = std::size_t;

	bit_array()
	{ }

	bit_array(const storage_t & s)
		: storage(s)
	{ }

	bit_array(storage_t && s)
		: storage(std::forward<storage_t>(s))
	{ }

#ifdef QT_CORE_LIB
	bit_array(const QByteArray & s)
		: storage(s.begin(), s.end())
	{ }

	bit_array& operator=(const QByteArray & s)
	{
		if(s.constData() != (const char*)&storage[0]) {
			storage = storage_t(s.begin(), s.end());
		}
		return *this;
	}
#endif

	bit_array& operator=(const bit_array & lhs) {
		if(this != &lhs) {
			storage = lhs.storage;
		}

		return *this;
	}

	bit_array& operator=(bit_array && lhs) {
		if(this != &lhs) {
			std::swap(storage, lhs.storage);
		}

		return *this;
	}

	size_t size() const {
		return storage.size() * 8;
	}

	bool empty() const {
		return storage.empty();
	}

	bool bit(int index) const {
		return (storage.at(index/8) & (1 << index%8)) > 0;
	}

	storage_t::const_reference byte(int index) const {
		return storage.at(index);
	}

	storage_t::reference byte(int index) {
		return storage.at(index);
	}

	void set(int index, bool value) {
		if(value)
			storage.at(index/8) |= (1 << (index%8));
		else
			storage.at(index/8) &= ~(1 << (index%8));
	}

	void resize(int s) {
		if(s < 0)
			return;

		storage.resize(s/8 + (s%8 ? 1 : 0));
	}

	void clear() {
		std::fill_n(storage.begin(), storage.size(), 0);
	}

	void clear(int begin) {
		int b = begin /8;
		int bit = begin % 8;
		if(bit) {
			int8_t m = 0;
			for(int i = 7; i >= bit; --i) {
				m |= 1 << i;
			}

			storage.at(b) &= ~m;
			b+=1;
		}

		std::fill_n(storage.begin() + b, storage.size() - b, 0);
	}

	bit_array& operator=(const storage_t & s) {
		storage = s;
		return *this;
	}

	bit_array& operator=(storage_t && s) {
		std::swap(storage, s);
		return *this;
	}

	bit_array& operator&=(const bit_array& lhs) {
		if(this != &lhs) {
			auto v1 = storage.begin();
			auto v2 = lhs.storage.begin();
			int max = std::min(storage.size(), lhs.storage.size());
			while(max-- > 0) {
				*v1++ &= *v2++;
			}

			std::for_each(v1, storage.end(), [](value_type & v){ v = 0; });
		}

		return *this;
	}

	bit_array& operator|=(const bit_array& lhs) {
		if(this != &lhs) {
			auto v1 = storage.begin();
			auto v2 = lhs.storage.begin();
			int max = std::min(storage.size(), lhs.storage.size());
			while(max-- > 0) {
				*v1++ |= *v2++;
			}

			std::for_each(v1, storage.end(), [](value_type & v){ v = 0; });
		}

		return *this;
	}

	bit_array& operator^=(const bit_array& lhs) {
		if(this != &lhs) {
			auto v1 = storage.begin();
			auto v2 = lhs.storage.begin();
			int max = std::min(storage.size(), lhs.storage.size());
			while(max-- > 0) {
				*v1++ ^= *v2++;
			}

			std::for_each(v1, storage.end(), [](value_type & v){ v = 0; });
		}

		return *this;
	}

	bit_array& operator&=(value_type lhs) {
		std::for_each(storage.begin(), storage.end(), [&lhs](value_type & v){ v &= lhs; });

		return *this;
	}

	bit_array& operator|=(value_type lhs) {
		std::for_each(storage.begin(), storage.end(), [&lhs](value_type & v){ v |= lhs; });

		return *this;
	}

	bit_array& operator^=(value_type lhs) {
		std::for_each(storage.begin(), storage.end(), [&lhs](value_type & v){ v ^= lhs; });

		return *this;
	}

	bool operator==(const bit_array & lhs) {
		if(this == &lhs)
			return true;

		return storage == lhs.storage;
	}

	bool operator!=(const bit_array & lhs) {
		if(this != &lhs)
			return true;

		return storage != lhs.storage;
	}

	bool operator[](int i) const {
		return bit(i);
	}

private:
	storage_t storage;
};

#endif
