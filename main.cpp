#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <tuple>

template<typename T, typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr >
void print_ip(T ip)
{
	for (size_t i = 1; i <= sizeof(ip); ++i)
	{
		std::cout << (unsigned int)(*(reinterpret_cast<unsigned char*>(&ip) + (sizeof(ip) - i))) << (i < sizeof(ip) ? "." : "\n");
	}
}

template<typename T, typename std::enable_if<std::is_same<T, std::string>::value, T>::type* = nullptr >
void print_ip(const T& ip)
{
	std::cout << ip << std::endl;
}

template<typename T, typename std::enable_if<std::is_same<T, std::vector<T>>::value, T>::type* = nullptr >
void print_ip(const T& ip)
{
	std::cout << ip << std::endl;
}

template <typename T>
struct is_container : std::false_type { };

template <typename T>
struct is_container<std::vector<T>> : std::true_type { };

template <typename T>
struct is_container<std::list<T>> : std::true_type { };

template<typename T, typename std::enable_if<is_container<T>::value, T>::type* = nullptr >
void print_ip(const T& ip)
{
	for (auto it = ip.begin(); it != ip.end(); ++it)
	{
		std::cout << *it << (it != --ip.end() ? "." : "\n");
	}
}

template <typename... T>
struct is_tuple : std::false_type { };

template <typename... T>
struct is_tuple<std::tuple<T ...>> : std::true_type { };

template<class Tuple, std::size_t N>
struct TuplePrinter {
	static void print(const Tuple& t)
	{
		TuplePrinter<Tuple, N - 1>::print(t);
		std::cout << "." << std::get<N - 1>(t);
	}
};

template<class Tuple>
struct TuplePrinter<Tuple, 1> {
	static void print(const Tuple& t)
	{
		std::cout << std::get<0>(t);
	}
};

template<class... Args>
void print_tuple(const std::tuple<Args...>& t)
{
	TuplePrinter<decltype(t), sizeof...(Args)>::print(t);
}

template<typename T, typename std::enable_if<is_tuple<T>::value, T>::type* = nullptr >
void print_ip(const T& ip)
{
	print_tuple(ip);
}

int main()
{	
	print_ip(int8_t{ -1 }); // 255 
	print_ip(int16_t{ 0 }); // 0.0 
	print_ip(int32_t{ 2130706433 }); // 127.0.0.1 
	print_ip(int64_t{ 8875824491850138409 });// 123.45.67.89.101.112.131.41 
	print_ip(std::string{ "Hello, World!" }); // Hello, World! 
	print_ip(std::vector<int>{100, 200, 300, 400}); // 100.200.300.400 
	print_ip(std::list<short>{400, 300, 200, 100}); // 400.300.200.100 
	print_ip( std::make_tuple(123, 456, 789, 0) ); // 123.456.789.0
	return 0;
}


