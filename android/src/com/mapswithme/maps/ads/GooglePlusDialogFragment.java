package com.mapswithme.maps.ads;

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v4.app.DialogFragment;
import android.view.LayoutInflater;
import android.view.View;

import com.google.android.gms.plus.PlusOneButton;
import com.mapswithme.maps.R;
import com.mapswithme.util.Constants;
import com.mapswithme.util.statistics.Statistics;

public class GooglePlusDialogFragment extends DialogFragment
{

  @Override
  public void onResume()
  {
    super.onResume();

    PlusOneButton plusButton = (PlusOneButton) getDialog().findViewById(R.id.btn__gplus);
    if (plusButton != null)
      plusButton.initialize(Constants.Url.PLAY_MARKET_HTTPS_APP_PREFIX + Constants.Package.MWM_PRO_PACKAGE, 0);
  }

  @NonNull
  @Override
  public Dialog onCreateDialog(Bundle savedInstanceState)
  {
    final AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
    final LayoutInflater inflater = getActivity().getLayoutInflater();

    final View root = inflater.inflate(R.layout.fragment_google_plus_dialog, null);
    builder.
        setView(root).
        setNegativeButton(getString(R.string.remind_me_later), new DialogInterface.OnClickListener()
        {
          @Override
          public void onClick(DialogInterface dialog, int which)
          {
            Statistics.INSTANCE.trackSimpleNamedEvent(Statistics.EventName.PLUS_DIALOG_LATER);
          }
        });

    return builder.create();
  }

  @Override
  public void onCancel(DialogInterface dialog)
  {
    super.onCancel(dialog);
    Statistics.INSTANCE.trackSimpleNamedEvent(Statistics.EventName.PLUS_DIALOG_LATER);
  }
}
