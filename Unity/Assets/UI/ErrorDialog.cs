using UnityEngine;
using UnityEngine.UI;

public class ErrorDialog : Dialog
{
	public Text text;

	public void SetErrorText(string textValue)
	{
		text.text = textValue;
	}
}
