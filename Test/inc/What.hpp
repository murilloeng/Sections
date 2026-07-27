#pragma once

class What
{
public:
	//constructor
	What(void);

	//destructor
	~What(void);

	//data
	bool nodes(bool);
	bool nodes(void) const;

	bool fibers(bool);
	bool fibers(void) const;

	bool elements(bool);
	bool elements(void) const;

private:
	//data
	bool m_nodes;
	bool m_fibers;
	bool m_elements;
};
