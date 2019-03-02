#include <iostream>
#include <vector>
#include <string>
#include "CVector.h"

struct President
{
	std::string name;
	std::string country;
	int year;

	President() {}

	President( std::string p_name, std::string p_country, int p_year )
		: name( std::move( p_name ) ), country( std::move( p_country ) ), year( p_year )
	{
		std::cout << "I am being constructed.\n";
	}
	President( President&& other )
		: name( std::move( other.name ) ), country( std::move( other.country ) ), year( other.year )
	{
		std::cout << "I am being moved.\n";
	}
	President& operator=( const President& other ) = default;
};

int main( int argc, char* argv[] )
{
	CVector<President> elections;
	std::cout << "emplace_back:\n";
	elections.emplace_back( "Nelson Mandela", "South Africa", 1994 );

	CVector<President> reElections;
	std::cout << "\npush_back:\n";
	reElections.push_back( President( "Franklin Delano Roosevelt", "the USA", 1936 ) );

	std::cout << "\nContents:\n";
	for ( size_t it = 0; it < elections.size(); it++ ) {
		std::cout << elections.at(it).name << " was elected president of "
			<< elections.at(it).country << " in " << elections.at(it).year << ".\n";
	}
	for ( size_t it = 0; it < reElections.size(); it++ ) {
		std::cout << reElections.at(it).name << " was re-elected president of "
			<< reElections.at(it).country << " in " << reElections.at(it).year << ".\n";
	}

    system("pause");
    return 0;
}
