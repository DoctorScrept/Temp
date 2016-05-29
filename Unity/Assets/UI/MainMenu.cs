using UnityEngine;
using UnityEngine.UI;

public class MainMenu : Dialog {

	public Button menuButton;

	public SaveLoadDialog saveLoaDialog;
	
//	void Start () {}
//	void Update () {}
	
	public void GoSave() {
		controler.PushDialog(saveLoaDialog);
	}

	public void GoLoad() {
		controler.PushDialog(saveLoaDialog);
	}

	public void ChangeTargetState() {
	}
	
	public void GoSettings() {
	}

	public override void SetIntarct(bool value) {
		base.SetIntarct(value);
		menuButton.interactable = value;
	}


}
