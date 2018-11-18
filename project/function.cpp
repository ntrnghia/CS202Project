#include "header.h"
map_type Factory::mymap;
Factory::Register<Line_> Line_::reg("line");
Factory::Register<Rect_> Rect_::reg("rect");
Factory::Register<Circle_> Circle_::reg("circle");
Factory::Register<Ellipse_> Ellipse_::reg("ellipse");
Factory::Register<Polyline_> Polyline_::reg("polyline");
Factory::Register<Polygon_> Polygon_::reg("polygon");
Factory::Register<Text_> Text_::reg("text");
Factory::Register<Path_> Path_::reg("path");
Factory::Register<Group_> Group_::reg("g");

void Attributes_::setAttribute(xml_node<> *node)
{
	for (xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute())
		attributes[attr->name()] = attr->value();
	attributes["text"] = node->value();
}
void Attributes_::setAttributeDefault(const char *attr_name, const char *attr_value)
{
	attributes[attr_name] = attr_value;
}

float Attributes_::getX()
{
	return stof(attributes["x"]);
}
float Attributes_::getY()
{
	return stof(attributes["y"]);
}
int Attributes_::getX1()
{
	return stoi(attributes["x1"]);
}
int Attributes_::getY1()
{
	return stoi(attributes["y1"]);
}
int Attributes_::getX2()
{
	return stoi(attributes["x2"]);
}
int Attributes_::getY2()
{
	return stoi(attributes["y2"]);
}
vector<int> Attributes_::getStroke()
{
	string stroke = attributes["stroke"];
	if (stroke == "" || stroke == "none")
		return vector<int>();
	vector<int> tmp;
	istringstream sin(stroke.c_str() + 4);
	for (int tmp2; sin >> tmp2; sin.get())
		tmp.push_back(tmp2);
	return tmp;
}
float Attributes_::getStrokeWidth()
{
	return stof(attributes["stroke-width"]);
}
int Attributes_::getStrokeOpacity()
{
	return int(stof(attributes["stroke-opacity"]) * 255);
}
float Attributes_::getWidth()
{
	return stof(attributes["width"]);
}
float Attributes_::getHeight()
{
	return stof(attributes["height"]);
}
vector<int> Attributes_::getFill()
{
	if (attributes["fill"] == "")
		return vector<int>(3, 0);
	if (attributes["fill"] == "none")
		return vector<int>(3, 255);
	vector<int> tmp;
	istringstream sin(attributes["fill"].c_str() + 4);
	for (int tmp2; sin >> tmp2; sin.get())
		tmp.push_back(tmp2);
	return tmp;
}
int Attributes_::getFillOpacity()
{
	return int(stof(attributes["fill-opacity"]) * 255);
}
int Attributes_::getCx()
{
	return stoi(attributes["cx"]);
}
int Attributes_::getCy()
{
	return stoi(attributes["cy"]);
}
int Attributes_::getR()
{
	return stoi(attributes["r"]);
}
int Attributes_::getRx()
{
	return stoi(attributes["rx"]);
}
int Attributes_::getRy()
{
	return stoi(attributes["ry"]);
}
vector<Point> Attributes_::getPoints()
{
	vector<Point> tmp;
	istringstream sin(attributes["points"]);
	for (Point tmp2; sin >> tmp2.X;)
	{
		sin.get();
		sin >> tmp2.Y;
		tmp.push_back(tmp2);
	}
	return tmp;
}
float Attributes_::getFontSize()
{
	return stof(attributes["font-size"]);
}
string Attributes_::getText()
{
	return attributes["text"];
}
vector<Attributes_::trans> Attributes_::getTransform()
{
	vector<trans> tmp;
	istringstream sin(attributes["transform"]);
	for (trans tmp2; getline(sin, tmp2.name, '('); tmp2.value.clear(), sin.get())
	{
		tmp2.name.erase(remove_if(tmp2.name.begin(), tmp2.name.end(), ::isspace), tmp2.name.end());
		string tmp3;
		getline(sin, tmp3, ')');
		istringstream sin2(tmp3);
		for (float tmp4; sin2 >> tmp4; sin2.get())
			tmp2.value.push_back(tmp4);
		tmp.push_back(tmp2);
	}
	return tmp;
}
vector<Attributes_::path> Attributes_::getD()
{
	vector<path> tmp;
	istringstream sin(attributes["d"]);
	for (char tmp2; sin >> tmp2;)
	{
		path tmp3;
		tmp3.command = tmp2;
		if (tmp2 == 'M' || tmp2 == 'L')
		{
			Point tmp4;
			sin >> tmp4.X;
			sin.get();
			sin >> tmp4.Y;
			tmp3.point.push_back(tmp4);
		}
		else if (tmp2 == 'C')
		{
			for (int i = 0; i < 3; ++i)
			{
				Point tmp4;
				sin >> tmp4.X;
				sin.get();
				sin >> tmp4.Y;
				tmp3.point.push_back(tmp4);
			}
		}
		tmp.push_back(tmp3);
	}
	return tmp;
}

Object_::~Object_()
{
	if (!pen)
		delete pen;
	if (!solidBrush)
		delete solidBrush;
}
void Object_::setAttribute(xml_node<> *node)
{
	attributes.setAttribute(node);
}
void Object_::copyAttribute(Object_ *object_)
{
	this->attributes = object_->attributes;
}
void Object_::SmoothAndTransform(Graphics &graphics)
{
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	vector<Attributes_::trans> transform = attributes.getTransform();
	for (int i = 0; i < (int)transform.size(); ++i)
	{
		if (transform[i].name == "scale")
			transform[i].value.size() == 2 ? graphics.ScaleTransform(transform[i].value[0], transform[i].value[1]) : graphics.ScaleTransform(transform[i].value[0], transform[i].value[0]);
		else if (transform[i].name == "rotate")
			graphics.RotateTransform(transform[i].value[0]);
		else if (transform[i].name == "translate")
			graphics.TranslateTransform(transform[i].value[0], transform[i].value[1]);
	}
}
Pen *Object_::getPen()
{
	vector<int> stroke = attributes.getStroke();
	pen = new Pen(Color(attributes.getStrokeOpacity(), stroke[0], stroke[1], stroke[2]), attributes.getStrokeWidth());
	return pen;
}
SolidBrush *Object_::getSolidBrush()
{
	vector<int> fill = attributes.getFill();
	solidBrush = new SolidBrush(Color(attributes.getFillOpacity(), fill[0], fill[1], fill[2]));
	return solidBrush;
}

Line_::Line_()
{
	attributes.setAttributeDefault("x1", "0");
	attributes.setAttributeDefault("y1", "0");
	attributes.setAttributeDefault("x2", "0");
	attributes.setAttributeDefault("y2", "0");
	attributes.setAttributeDefault("stroke", "");
	attributes.setAttributeDefault("stroke-width", "0");
	attributes.setAttributeDefault("stroke-opacity", "1");
	attributes.setAttributeDefault("transform", "");
}
void Line_::Draw(const HDC &hdc, xml_node<> *node)
{
	Graphics graphics(hdc);
	SmoothAndTransform(graphics);
	if (attributes.getStroke().size())
		graphics.DrawLine(getPen(), attributes.getX1(), attributes.getY1(), attributes.getX2(), attributes.getY2());
}

Rect_::Rect_()
{
	attributes.setAttributeDefault("x", "0");
	attributes.setAttributeDefault("y", "0");
	attributes.setAttributeDefault("width", "0");
	attributes.setAttributeDefault("height", "0");
	attributes.setAttributeDefault("stroke", "");
	attributes.setAttributeDefault("stroke-width", "0");
	attributes.setAttributeDefault("stroke-opacity", "1");
	attributes.setAttributeDefault("fill", "rbg(0,0,0)");
	attributes.setAttributeDefault("fill-opacity", "1");
	attributes.setAttributeDefault("transform", "");
}
void Rect_::Draw(const HDC &hdc, xml_node<> *node)
{
	Graphics graphics(hdc);
	SmoothAndTransform(graphics);
	graphics.FillRectangle(getSolidBrush(), attributes.getX(), attributes.getY(), attributes.getWidth(), attributes.getHeight());
	if (attributes.getStroke().size())
		graphics.DrawRectangle(getPen(), attributes.getX(), attributes.getY(), attributes.getWidth(), attributes.getHeight());
}

Circle_::Circle_()
{
	attributes.setAttributeDefault("cx", "0");
	attributes.setAttributeDefault("cy", "0");
	attributes.setAttributeDefault("r", "0");
	attributes.setAttributeDefault("stroke", "");
	attributes.setAttributeDefault("stroke-width", "0");
	attributes.setAttributeDefault("stroke-opacity", "1");
	attributes.setAttributeDefault("fill", "rbg(0,0,0)");
	attributes.setAttributeDefault("fill-opacity", "1");
	attributes.setAttributeDefault("transform", "");
}
void Circle_::Draw(const HDC &hdc, xml_node<> *node)
{
	Graphics graphics(hdc);
	SmoothAndTransform(graphics);
	graphics.FillEllipse(getSolidBrush(), attributes.getCx() - attributes.getR(), attributes.getCy() - attributes.getR(), 2 * attributes.getR(), 2 * attributes.getR());
	if (attributes.getStroke().size())
		graphics.DrawEllipse(getPen(), attributes.getCx() - attributes.getR(), attributes.getCy() - attributes.getR(), 2 * attributes.getR(), 2 * attributes.getR());
}

Ellipse_::Ellipse_()
{
	attributes.setAttributeDefault("cx", "0");
	attributes.setAttributeDefault("cy", "0");
	attributes.setAttributeDefault("rx", "0");
	attributes.setAttributeDefault("ry", "0");
	attributes.setAttributeDefault("stroke", "");
	attributes.setAttributeDefault("stroke-width", "0");
	attributes.setAttributeDefault("stroke-opacity", "1");
	attributes.setAttributeDefault("fill", "rbg(0,0,0)");
	attributes.setAttributeDefault("fill-opacity", "1");
	attributes.setAttributeDefault("transform", "");
}
void Ellipse_::Draw(const HDC &hdc, xml_node<> *node)
{
	Graphics graphics(hdc);
	SmoothAndTransform(graphics);
	graphics.FillEllipse(getSolidBrush(), attributes.getCx() - attributes.getRx(), attributes.getCy() - attributes.getRy(), 2 * attributes.getRx(), 2 * attributes.getRy());
	if (attributes.getStroke().size())
		graphics.DrawEllipse(getPen(), attributes.getCx() - attributes.getRx(), attributes.getCy() - attributes.getRy(), 2 * attributes.getRx(), 2 * attributes.getRy());
}

Polyline_::Polyline_()
{
	attributes.setAttributeDefault("points", "0");
	attributes.setAttributeDefault("stroke", "");
	attributes.setAttributeDefault("stroke-width", "0");
	attributes.setAttributeDefault("stroke-opacity", "1");
	attributes.setAttributeDefault("fill", "rbg(0,0,0)");
	attributes.setAttributeDefault("fill-opacity", "1");
	attributes.setAttributeDefault("transform", "");
}
void Polyline_::Draw(const HDC &hdc, xml_node<> *node)
{
	Graphics graphics(hdc);
	SmoothAndTransform(graphics);
	graphics.FillPolygon(getSolidBrush(), &attributes.getPoints()[0], (int)attributes.getPoints().size());
	if (attributes.getStroke().size())
	{
		GraphicsPath path;
		path.AddLines(&attributes.getPoints()[0], (int)attributes.getPoints().size());
		graphics.DrawPath(getPen(), &path);
	}
}

Polygon_::Polygon_()
{
	attributes.setAttributeDefault("points", "0");
	attributes.setAttributeDefault("stroke", "");
	attributes.setAttributeDefault("stroke-width", "0");
	attributes.setAttributeDefault("stroke-opacity", "1");
	attributes.setAttributeDefault("fill", "rbg(0,0,0)");
	attributes.setAttributeDefault("fill-opacity", "1");
	attributes.setAttributeDefault("transform", "");
}
void Polygon_::Draw(const HDC &hdc, xml_node<> *node)
{
	Graphics graphics(hdc);
	SmoothAndTransform(graphics);
	graphics.FillPolygon(getSolidBrush(), &attributes.getPoints()[0], (int)attributes.getPoints().size());
	if (attributes.getStroke().size())
		graphics.DrawPolygon(getPen(), &attributes.getPoints()[0], (int)attributes.getPoints().size());
}

Text_::Text_()
{
	attributes.setAttributeDefault("x", "0");
	attributes.setAttributeDefault("y", "0");
	attributes.setAttributeDefault("text", "");
	attributes.setAttributeDefault("font-size", "0");
	attributes.setAttributeDefault("stroke", "");
	attributes.setAttributeDefault("stroke-width", "0");
	attributes.setAttributeDefault("stroke-opacity", "1");
	attributes.setAttributeDefault("fill", "rbg(0,0,0)");
	attributes.setAttributeDefault("fill-opacity", "1");
	attributes.setAttributeDefault("transform", "");
}
void Text_::Draw(const HDC &hdc, xml_node<> *node)
{
	Graphics graphics(hdc);
	SmoothAndTransform(graphics);
	FontFamily fontFamily(L"Times New Roman");
	Font font(&fontFamily, attributes.getFontSize(), FontStyleRegular, UnitPixel);
	PointF pointF(float(attributes.getX() - 0.17 * attributes.getFontSize()), float(attributes.getY() - 0.9 * attributes.getFontSize()));
	string text = attributes.getText();
	graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
	graphics.DrawString(wstring(text.begin(), text.end()).c_str(), (int)text.size(), &font, pointF, getSolidBrush());
	if (attributes.getStroke().size())
	{
		GraphicsPath path;
		path.AddString(wstring(text.begin(), text.end()).c_str(), (int)text.size(), &fontFamily, FontStyleRegular, attributes.getFontSize(), pointF, NULL);
		graphics.DrawPath(getPen(), &path);
	}
}

Path_::Path_()
{
	attributes.setAttributeDefault("d", "");
	attributes.setAttributeDefault("stroke", "");
	attributes.setAttributeDefault("stroke-width", "0");
	attributes.setAttributeDefault("stroke-opacity", "1");
	attributes.setAttributeDefault("fill", "rbg(0,0,0)");
	attributes.setAttributeDefault("fill-opacity", "1");
	attributes.setAttributeDefault("transform", "");
}
void Path_::Draw(const HDC &hdc, xml_node<> *node)
{
	Graphics graphics(hdc);
	SmoothAndTransform(graphics);
	vector<Attributes_::path> d = attributes.getD();
	vector<Point> all;
	GraphicsPath path;
	Point current;
	for (int i = 0; i < (int)d.size(); ++i)
	{
		if (d[i].command == 'M')
			current = d[i].point[0];
		else if (d[i].command == 'L')
		{
			path.AddLine(current, d[i].point[0]);
			current = d[i].point[0];
		}
		else if (d[i].command == 'C')
		{
			path.AddBezier(current, d[i].point[0], d[i].point[1], d[i].point[2]);
			current = d[i].point[2];
		}
		else if (d[i].command == 'z')
			path.AddLine(current, d[0].point[0]);
		all.insert(all.end(), d[i].point.begin(), d[i].point.end());
	}
	graphics.FillPath(getSolidBrush(), &path);
	graphics.DrawPath(getPen(), &path);
}

Group_::Group_()
{
	attributes.setAttributeDefault("stroke", "");
	attributes.setAttributeDefault("stroke-width", "0");
	attributes.setAttributeDefault("stroke-opacity", "1");
	attributes.setAttributeDefault("fill", "rbg(0,0,0)");
	attributes.setAttributeDefault("fill-opacity", "1");
	attributes.setAttributeDefault("transform", "");
}
void Group_::Draw(const HDC &hdc, xml_node<> *node)
{
	Graphics graphics(hdc);
	SmoothAndTransform(graphics);
	for (xml_node<> *node2 = node->first_node(); node2; node2 = node2->next_sibling())
	{
		Object_ *cur = Factory::createInstance(node2->name());
		cur->copyAttribute(this);
		cur->setAttribute(node2);
		cur->Draw(hdc, node2);
		delete cur;
	}
}