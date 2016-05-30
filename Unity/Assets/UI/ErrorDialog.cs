using UnityEngine;
using UnityEngine.UI;

public class ErrorDialog : Dialog
{
	public Text text;

	void Awake() {
		text.color = Color.red;
	}

	public void SetErrorText(string textValue)
	{
		text.text = textValue;
	}
}
