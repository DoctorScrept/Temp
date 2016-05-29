using UnityEngine;
using UnityEngine.UI;

public class SaveLoadDialog : Dialog {

	public Button deviceTab;
	public Button fileTab;

	public GameObject devicePage;
	public GameObject filePage;
	
//	void Start () {}
//	void Update () {}

	public void GoDevice() {
		devicePage.SetActive(true);
		filePage.SetActive(false);
	}

	public void GoFile() {
		devicePage.SetActive(false);
		filePage.SetActive(true);
	}
}
