using UnityEngine;
using System.Collections;

public class Dialog : MonoBehaviour {

	private CanvasGroup group;

	public static CurveTracer3DControler controler;

//
//	void Start () {}
//	void Update () {}

	public void Close() {
		controler.PopDialog();
	}

	public CanvasGroup GetCanvasGroup() {
		if (group == null) {
			group = GetComponent<CanvasGroup>();
		}
		return group;
	}

	public virtual void SetIntarct(bool value) {
		GetCanvasGroup().interactable = value;
	}
}
