#pragma once
#include <string>
#include <stddef.h>
#include "gd.h"

#include <string>
#include <string_view>

namespace gd {

struct string {
	union {
		char inline_data[16];
		char* ptr;
	} m_data;
	size_t m_size = 0;
	size_t m_capacity = 15;

	// TODO:
	// ~string() = delete;
	~string() {
		if (m_capacity > 15) {
			delete m_data.ptr;
			m_capacity = 15;
		}
		m_data.ptr = nullptr;
		m_size = 0;
	}

	string(const char* data, size_t size) {
		// reinterpret_cast<void*(__thiscall*)(void*, const char*, size_t)>(base + 0xf840)(this, data, size);
		this->assign(data, size);
	}

	size_t size() const { return m_size; }

	string& assign(const char* data, size_t size) {
		if (m_capacity > 15) {
			delete m_data.ptr;
			m_capacity = 15;
			m_data.inline_data[0] = 0;
			m_size = 0;
		}
		reinterpret_cast<void*(__thiscall*)(void*, const char*, size_t)>(gd::base + 0xf840)(this, data, size);
		return *this;
	}

	string& operator=(const std::string& other) {
		return this->assign(other.c_str(), other.size());
	}

	string& operator=(const string& other) {
		return this->assign(other.c_str(), other.size());
	}

	char* data() {
		if (m_capacity <= 15) return m_data.inline_data;
		return m_data.ptr;
	}

	const char* data() const { return c_str(); }

	const char* c_str() const {
		if (m_capacity <= 15) return m_data.inline_data;
		return m_data.ptr;
	}

	std::string to_str() const {
		return std::string(c_str(), size());
	}

	std::string_view to_sv() const {
		return std::string_view(c_str(), size());
	}

	operator std::string() const { return to_str(); }
	operator std::string_view() const { return to_sv(); }
};

static_assert(sizeof(string) == 24, "sizeof(gd::string) != 24, something went wrong..");

}