using UnityEngine;
using UnityEngine.UI;
using System.Collections.Generic;

public class CurveTracer3DControler : MonoBehaviour
{
	public TracerDevice device;
	public Plotter plotter;
	
	public MainMenu mainMenu;
	public ErrorDialog errorDialog;


	public Button startMeasureButton;
	public Text completeText;


	private Stack<Dialog> dialogs = new Stack<Dialog>();

	private int lastPersent = 0;
	private float[,] defaultData = { 	
		{0f, 0f, 0f, 0f},
		{0f, 0f, 0f, 0f},
		{0f, 0f, 2f, 0f},
		{0f, 0f, 0f, 1f}};
	
	void Start () {
		Dialog.controler = this;
		mainMenu.markerControler.plotter = plotter;

		Surface s = new Surface(4, 4);
		s.SetData(defaultData);
		plotter.DrawToSlot(0, s);
	}


	void Update ()
	{
		int complelte = device.GetPercentComplete();
		if (lastPersent != complelte) {
			lastPersent = complelte;
			completeText.text = complelte.ToString() + "%";
		}

		float[] data = device.GetData();
		if (data != null) {
		    Surface s = new Surface(4, 4);
			s.SetData(data);
			plotter.DrawToSlot(0, s);

			startMeasureButton.gameObject.SetActive(true);
			completeText.gameObject.SetActive(false);
			completeText.text = "0%";
			CloseAll();
		}
	}

	public void StatMeasure()
	{
		if (!device.StatMeasure()) {
			string errorText = device.GetErrorText();
			PushDialog(errorDialog);
			errorDialog.SetErrorText(errorText);
		} else {
			startMeasureButton.gameObject.SetActive(false);
			completeText.gameObject.SetActive(true);
		}
	}

	public void StatMeasureTest()
	{
		if (!device.StatMeasure()) {
			string errorText = device.GetErrorText();
			Debug.LogError(errorText);
			PushDialog(errorDialog);
			errorDialog.SetErrorText(errorText);
		}
	}
	
	public void StopMeasure() {
		device.StopMeasure();
	}
	
	public void OpenCloseMenu()
	{
		if (dialogs.Count > 0) {
			PopDialog();
		} else {
			PushDialog(mainMenu);
		}
	}


	public void PushDialog(Dialog dialog)
	{
		if (dialogs.Count > 0) {
			Dialog prev = dialogs.Peek();
			prev.SetIntarct(false);
		}

		dialogs.Push(dialog);
		dialog.gameObject.SetActive(true);
	}

	public Dialog PopDialog()
	{
		Dialog dialog = dialogs.Pop();
		dialog.gameObject.SetActive(false);

		if (dialogs.Count > 0) {
			Dialog prev = dialogs.Peek();
			prev.SetIntarct(true);
		}

		return dialog;
	}

	public void CloseAll() {
		while (dialogs.Count > 0) {
			PopDialog();
		}
	}

}
