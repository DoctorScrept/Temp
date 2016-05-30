using UnityEngine;
using UnityEngine.UI;

public class MainMenu : Dialog
{
	public Button menuButton;

	public SaveLoadDialog saveLoaDialog;
	public MarkerControler markerControler;
	public Button markerButton;
	public Sprite hilightedMarkerButton;
	private Sprite normalMarkerButton;
	
//	void Start () {}
//	void Update () {}
	void Awake() {
		Image targetBuutonImage = markerButton.gameObject.GetComponent<Image>();
		normalMarkerButton = targetBuutonImage.sprite;
	}

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
		Image targetBuutonImage = markerButton.gameObject.GetComponent<Image>();
		if (markerControler.IsEnable) {
			targetBuutonImage.sprite = hilightedMarkerButton;
		} else {
			targetBuutonImage.sprite = normalMarkerButton;
		}
	}
	
	public void GoSettings() {
	}

	public override void SetIntarct(bool value) {
		base.SetIntarct(value);
		menuButton.interactable = value;
	}


}
