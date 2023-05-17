#pragma once

#include <string.h>

#include <string>
#include <vector>

namespace dos {
	class string : public std::string
	{
	public:

		string() : std::string("")
		{
		}
		string(char * _value) : std::string(_value)
		{
		}

		string(const char* _value) : std::string(_value)
		{
		}

		string(char _value) : std::string( std::to_string(_value) )
		{
		}
		string(unsigned char _value) : std::string(std::to_string(_value))
		{
		}
		string(short _value) : std::string(std::to_string(_value))
		{
		}
		string(unsigned short _value) : std::string(std::to_string(_value))
		{
		}
		string(int _value) : std::string(std::to_string(_value))
		{
		}
		string(unsigned int _value) : std::string(std::to_string(_value))
		{
		}
		string(long _value) : std::string(std::to_string(_value))
		{
		}
		string(unsigned long _value) : std::string(std::to_string(_value))
		{
		}
		string(long long _value) : std::string(std::to_string(_value))
		{
		}
		string(float _value) : std::string(std::to_string(_value))
		{
		}
		string(double _value) : std::string(std::to_string(_value))
		{
		}
		string(const std::string& _value) : std::string( _value )
		{
		}
		string(const string& _value) : std::string( _value )
		{
		}

		string& append(const string& other)
		{
			std::string::append( other.toString() );
			return *this;
		}

		const int length() const noexcept { return (int)std::string::length(); }

		const char* toString() const { return (char* const)std::string::c_str(); }

		const char toChar() const { return (char const)toString()[0]; }

		const unsigned char toUChar() const { return (unsigned char const)toString()[0]; }

		const short toShort() const { return (short const)std::atoi(toString()); }

		const short toUShort() const { return (short const)std::atoi(toString()); }

		const int toInt() const { return (int const)std::atoi(toString()); }

		const unsigned int toUInt() const { return (int const)std::atol(toString()); }

		const long toLong() const { return (int const)std::atol(toString()); }

		const unsigned long toULong() const { return (int const)std::atol(toString()); }

		const long long toLongLong() const { return (int const)std::atoll( toString() ); }
		
		const float toFloat() const { return (float const)std::atof( toString() ); }
		const double toDouble() const { return (double const)std::atof( toString() ); }

		std::vector<string> split(const string& parser)
		{
			char tmp[4096];
			std::vector<string> s;

			s.clear();
			const char* str = parser.toString();
			int len = parser.length();

			//printf("\n\n==============\n");
			//printf("split(const string& parser) >> row: %s\n", toString());

			for (;;)
			{
				int pos = find(str);
				if (pos == -1)
					break;

				memcpy( tmp, toString(), pos);
				tmp[pos] = '\0';
				s.push_back( tmp );

				//printf("column >> %d,%s,%d >> %s\n", parser.length(), parser.toString(), pos, tmp);

				std::string() = erase( 0, pos+len );

				//printf("cur column >> %s\n", toString());
			}


			return s;
		}

	private:


	public:

		bool operator==(string& other) const
		{
			return 
				( length() == other.length() ) &&
				( strcmp( toString(), other.toString() ) == 0 )
				;
		}

		string& operator+(string& other)
		{
			append(other);
			return *this;
		}
	};
}