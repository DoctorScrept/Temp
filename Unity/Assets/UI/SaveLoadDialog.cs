using UnityEngine;
using UnityEngine.UI;

public class SaveLoadDialog : Dialog {

	public Button deviceTab;
	public Button fileTab;

	public GameObject devicePage;
	public GameObject filePage;

	public Text saveLoadFileButtonText;

//	void Start () {}
//	void Update () {}

	public void SetState(bool isSave) {
		if (isSave) {
			saveLoadFileButtonText.text = "Save";
			deviceTab.gameObject.SetActive(false);
			GoFile();
		} else {
			saveLoadFileButtonText.text = "Load";
			deviceTab.gameObject.SetActive(true);
			GoDevice();
		}
	}

	public void GoDevice() {
		devicePage.SetActive(true);
		filePage.SetActive(false);
	}

	public void GoFile() {
		devicePage.SetActive(false);
		filePage.SetActive(true);
	}
}
