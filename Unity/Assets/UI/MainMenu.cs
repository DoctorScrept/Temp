using UnityEngine;
using UnityEngine.UI;

public class MainMenu : Dialog
{
	public Button menuButton;

	public SaveLoadDialog saveLoaDialog;
	public MarkerControler markerControler;
	
//	void Start () {}
//	void Update () {}
	
	public void GoSave() {
		saveLoaDialog.SetState(true);
		controler.PushDialog(saveLoaDialog);
	}

	public void GoLoad() {
		saveLoaDialog.SetState(false);
		controler.PushDialog(saveLoaDialog);
	}

	public void ChangeTargetState() {
		markerControler.IsEnable = !markerControler.IsEnable;
	}
	
	public void GoSettings() {
	}

	public override void SetIntarct(bool value) {
		base.SetIntarct(value);
		menuButton.interactable = value;
	}


}
