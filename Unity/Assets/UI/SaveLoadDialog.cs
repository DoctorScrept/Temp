using UnityEngine;
using UnityEngine.UI;

public class SaveLoadDialog : Dialog {

	public Button deviceTab;
	public Button fileTab;

	public GameObject devicePage;
	public GameObject filePage;

	public Text saveLoadFileButtonText;

	public void SetState(bool isSave) {
		if (isSave) {
			saveLoadFileButtonText.text = "Зберегти";
			deviceTab.gameObject.SetActive(false);
			GoFile();
		} else {
			saveLoadFileButtonText.text = "Відкрити";
			deviceTab.gameObject.SetActive(true);
			GoDevice();
		}
	}

	public void GoDevice()
	{
		SetTabAlpha(deviceTab, 1f);
		SetTabAlpha(fileTab, 0.5f);

		devicePage.SetActive(true);
		filePage.SetActive(false);
	}

	public void GoFile()
	{
		SetTabAlpha(deviceTab, 0.5f);
		SetTabAlpha(fileTab, 1f);

		devicePage.SetActive(false);
		filePage.SetActive(true);
	}

	private void SetTabAlpha(Button tab, float a)
	{
		ColorBlock cb = tab.colors;
		cb.normalColor = new Color(1f, 1f, 1f, a);
		tab.colors = cb;
	}
}
