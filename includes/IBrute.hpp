/**
    @author Louis JURCZYK
    @version 1.0 06/03/19
*/

#ifndef         _IBRUTE_HPP_
#define         _IBRUTE_HPP_

class   IBrute
{
public:
    IBrute();
    ~IBrute();

    virtual void	brute(void) = 0;
    /*
	while (true){
		if (_cracker->crack("bite"))
			"password trouvé!"
	}

    */
    void			setCracker(ICracker *c) {
    	_cracker = c;
    }

private:
	ICracker		*_cracker;
};
#endif      /*_IBRUTE_HPP_*/