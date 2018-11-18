#pragma once
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <windows.h>
#include <gdiplus.h>
#include "rapidxml-1.13/rapidxml.hpp"
using namespace std;
using namespace Gdiplus;
using namespace rapidxml;
#pragma comment(lib, "Gdiplus.lib")

class Attributes_
{
public:
	void setAttribute(xml_node<> *node);
	void setAttributeDefault(const char *attr_name, const char *attr_value);

	float getX();
	float getY();
	int getX1();
	int getY1();
	int getX2();
	int getY2();
	vector<int> getStroke();
	float getStrokeWidth();
	int getStrokeOpacity();
	float getWidth();
	float getHeight();
	vector<int> getFill();
	int getFillOpacity();
	int getCx();
	int getCy();
	int getR();
	int getRx();
	int getRy();
	vector<Point> getPoints();
	float getFontSize();
	string getText();
	struct trans
	{
		string name;
		vector<float> value;
	};
	vector<trans> getTransform();
	struct path
	{
		char command;
		vector<Point> point;
	};
	vector<path> getD();

private:
	map<string, string> attributes;
};

class Object_
{
public:
	~Object_();
	virtual void Draw(const HDC &hdc, xml_node<> *node) = 0;
	void setAttribute(xml_node<> *node);
	void copyAttribute(Object_ *object_);

protected:
	Attributes_ attributes;
	void SmoothAndTransform(Graphics &graphics);
	Pen *getPen();
	SolidBrush *getSolidBrush();

private:
	Pen * pen;
	SolidBrush *solidBrush;
};

template <typename T>
Object_ *createT()
{
	return new T;
}
typedef map<string, Object_ *(*)()> map_type;
class Factory
{
public:
	static Object_ *createInstance(string s)
	{
		return mymap[s]();
	}
	template <typename T>
	struct Register
	{
		Register(string s)
		{
			mymap[s] = &createT<T>;
		}
	};

protected:
	static map_type mymap;
};

class Line_ : public Object_
{
public:
	Line_();
	virtual void Draw(const HDC &hdc, xml_node<> *node);

private:
	static Factory::Register<Line_> reg;
};
class Rect_ : public Object_
{
public:
	Rect_();
	virtual void Draw(const HDC &hdc, xml_node<> *node);

private:
	static Factory::Register<Rect_> reg;
};
class Circle_ : public Object_
{
public:
	Circle_();
	virtual void Draw(const HDC &hdc, xml_node<> *node);

private:
	static Factory::Register<Circle_> reg;
};
class Ellipse_ : public Object_
{
public:
	Ellipse_();
	virtual void Draw(const HDC &hdc, xml_node<> *node);

private:
	static Factory::Register<Ellipse_> reg;
};
class Polyline_ : public Object_
{
public:
	Polyline_();
	virtual void Draw(const HDC &hdc, xml_node<> *node);

private:
	static Factory::Register<Polyline_> reg;
};
class Polygon_ : public Object_
{
public:
	Polygon_();
	virtual void Draw(const HDC &hdc, xml_node<> *node);

private:
	static Factory::Register<Polygon_> reg;
};
class Text_ : public Object_
{
public:
	Text_();
	virtual void Draw(const HDC &hdc, xml_node<> *node);

private:
	static Factory::Register<Text_> reg;
};
class Path_ : public Object_
{
public:
	Path_();
	virtual void Draw(const HDC &hdc, xml_node<> *node);

private:
	static Factory::Register<Path_> reg;
};
class Group_ : public Object_
{
public:
	Group_();
	virtual void Draw(const HDC &hdc, xml_node<> *node);

private:
	static Factory::Register<Group_> reg;
};