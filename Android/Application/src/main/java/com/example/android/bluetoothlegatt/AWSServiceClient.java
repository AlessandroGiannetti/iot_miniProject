package com.example.android.bluetoothlegatt;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;
import android.util.Log;
import android.view.View;

import com.amazonaws.mobile.client.AWSMobileClient;
import com.amazonaws.mobile.client.Callback;
import com.amazonaws.mobile.client.UserStateDetails;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttClientStatusCallback;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttManager;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttNewMessageCallback;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttQos;

import java.io.UnsupportedEncodingException;
import java.nio.charset.StandardCharsets;
import java.util.Date;
import java.util.UUID;
import java.util.concurrent.CountDownLatch;


public class AWSServiceClient {

    // amazon config
    private static final String CUSTOMER_SPECIFIC_IOT_ENDPOINT = "a38hrniipnig3a-ats.iot.us-east-1.amazonaws.com";
    AWSIotMqttManager mqttManager;
    String clientId;

    private final static String TAG = DeviceControlActivity.class.getSimpleName();

    private static AWSServiceClient instance;

    public static Context cxt;

    public static synchronized AWSServiceClient getInstance(){
        if(instance ==null)
            instance = new AWSServiceClient();
        return instance;
    }

    private AWSServiceClient(){

    }

    // AWS METHOD
    public void connect() {
        // AWS connection
        clientId = UUID.randomUUID().toString();
        final CountDownLatch latch = new CountDownLatch(1);
        AWSMobileClient.getInstance().initialize(cxt,
//                DeviceControlActivity.getInstance().getApplicationContext(),
                new Callback<UserStateDetails>() {
                    @Override
                    public void onResult(UserStateDetails result) {
                        latch.countDown();
                    }

                    @Override
                    public void onError(Exception e) {
                        latch.countDown();
                        Log.e(TAG, "onError: ", e);
                    }
                }
        );

        try {
            latch.await();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        // MQTT Client
        mqttManager = new AWSIotMqttManager(clientId, CUSTOMER_SPECIFIC_IOT_ENDPOINT);

        Log.d(TAG, "clientId = " + clientId);

        try {
            mqttManager.connect(AWSMobileClient.getInstance(), new AWSIotMqttClientStatusCallback() {
                @Override
                public void onStatusChanged(final AWSIotMqttClientStatus status,
                                            final Throwable throwable) {
                    Log.d(TAG, "Status AWS IoT= " + String.valueOf(status));
                    new Runnable() {
                        @Override
                        public void run() {
                            if (throwable != null) {
                                Log.e(TAG, "Connection AWS error.", throwable);
                            }
                        }
                    };
                }
            });
        } catch (final Exception e) {
            Log.e(TAG, "Connection AWS. error.", e);
        }
    }

    public void subscribe(final View view) {
        final String topic = "my/iotminiproject";

        Log.d(TAG, "topic = " + topic);

        try {
            mqttManager.subscribeToTopic(topic, AWSIotMqttQos.QOS0,
                    new AWSIotMqttNewMessageCallback() {
                        @Override
                        public void onMessageArrived(final String topic, final byte[] data) {
                            new Runnable() {
                                @Override
                                public void run() {
                                    String message = new String(data, StandardCharsets.UTF_8);
                                    Log.d(TAG, "Message arrived:");
                                    Log.d(TAG, "   Topic: " + topic);
                                    Log.d(TAG, " Message: " + message);


                                }
                            };
                        }
                    });
        } catch (Exception e) {
            Log.e(TAG, "Subscription error.", e);
        }
    }

    public void publish(String value) {

        try {
            mqttManager.publishString("{\"number\" : \"" + new Date().toString().substring(0,20) + "\", \"type\": \"" +value+"\"}", "my/iotminiproject", AWSIotMqttQos.QOS0);
        } catch (Exception e) {
            // Log.e(TAG, "Publish error.", e);
        }
    }

    public void disconnect() {
        try {
            mqttManager.disconnect();
        } catch (Exception e) {
            Log.e(TAG, "Disconnect error.", e);
        }
    }

}
