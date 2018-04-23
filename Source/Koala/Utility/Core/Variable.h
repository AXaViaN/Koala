#ifndef KOALA__UTILITY__CORE__VARIABLE
#define KOALA__UTILITY__CORE__VARIABLE

#include <Koala/Utility/Resource.h>
#include <string>

namespace Koala::Utility::Core {

enum class VariableType;

class Variable
{
public:
	Variable(const std::string& name, VariableType type) : 
		m_Name(name),
		m_Type(type)
	{ }
	Variable(Text nameText, VariableType type) : 
		m_NameText(nameText),
		m_Type(type)
	{ }

	const std::string& GetName() const
	{
		if(m_NameText == Text::Empty)
		{
			return m_Name;
		}
		else
		{
			return Resource::GetText(m_NameText);
		}
	}
	const VariableType& GetVariableType() const
	{
		return m_Type;
	}

	void SetValueFloat64(double value)
	{
		m_ValueFloat64 = value;
	}
	double GetValueFloat64() const
	{
		return m_ValueFloat64;
	}
	void SetValueString(const std::string& value)
	{
		m_ValueString = value;
	}
	const std::string& GetValueString() const
	{
		return m_ValueString;
	}

private:
	std::string m_Name;
	Text m_NameText = Text::Empty;
	VariableType m_Type;

	double m_ValueFloat64 = 0.0f;
	std::string m_ValueString;

};

enum class VariableType
{
	None,

	Float64,
	String
};

} // namespace Koala::Utility::Core

#endif // KOALA__UTILITY__CORE__VARIABLE
