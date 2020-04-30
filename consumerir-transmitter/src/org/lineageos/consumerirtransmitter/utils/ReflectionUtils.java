/*
 * Copyright (c) 2018 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.lineageos.consumerirtransmitter.utils;

import android.content.Context;
import android.text.TextUtils;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public class ReflectionUtils {
    private static final String TAG = "ReflectionUtil";

    private ReflectionUtils() {}

    public static Object invokeMethod(
        Object instance, String methodName, Class[] argsClass, Object[] args) {
        if (instance == null || TextUtils.isEmpty(methodName)) {
            return null;
        }

        if (argsClass != null && args != null && argsClass.length != args.length) {
            return null;
        }

        Class<? extends Object> clazz = instance.getClass();
        Method method = null;
        try {
            if (argsClass == null || args == null) {
                method = clazz.getMethod(methodName);
            } else {
                method = clazz.getMethod(methodName, argsClass);
            }
        } catch (SecurityException e) {
            Log.d(TAG, null, e);
            return null;
        } catch (NoSuchMethodException e) {
            Log.d(TAG, null, e);
            return null;
        }
        try {
            if (method != null) {
                if (argsClass == null || args == null) {
                    return method.invoke(instance);
                } else {
                    return method.invoke(instance, args);
                }
            }
        } catch (IllegalArgumentException e) {
            Log.d(TAG, null, e);
        } catch (IllegalAccessException e) {
            Log.d(TAG, null, e);
        } catch (InvocationTargetException e) {
            Log.d(TAG, null, e);
        }
        return null;
    }

    public static Object invokeStaticMethod(
        String clazzName, String methodName, Class[] argsClass, Object[] args) {
        if (TextUtils.isEmpty(clazzName) || TextUtils.isEmpty(methodName)) {
            return null;
        }

        if (argsClass != null && args != null && argsClass.length != args.length) {
            return null;
        }

        Class clazz = null;
        try {
            clazz = Class.forName(clazzName);
        } catch (ClassNotFoundException e) {
        }
        if (null == clazz) {
            return null;
        }

        Method method = null;

        try {
            if (argsClass == null || args == null) {
                method = clazz.getMethod(methodName);
            } else {
                method = clazz.getMethod(methodName, argsClass);
            }
        } catch (NoSuchMethodException e) {
            Log.d(TAG, null, e);
            return null;
        }

        try {
            if (method != null) {
                if (argsClass == null || args == null) {
                    return method.invoke(clazz);
                } else {
                    return method.invoke(clazz, args);
                }
            }
        } catch (IllegalAccessException | IllegalArgumentException | InvocationTargetException e) {
            // TODO Auto-generated catch block
            Log.d(TAG, null, e);
        }

        return null;
    }

    public static Object getField(Object instance, String fieldName) {
        if (instance == null || TextUtils.isEmpty(fieldName)) {
            return null;
        }
        Field field = null;
        try {
            field = instance.getClass().getField(fieldName);
            return field;
        } catch (NoSuchFieldException e) {
            Log.d(TAG, null, e);
        }
        return null;
    }
    public static Object getField(Class clazz, String fieldName) {
        if (clazz == null || TextUtils.isEmpty(fieldName)) {
            return null;
        }
        Field field = null;
        try {
            field = clazz.getField(fieldName);
            return field;
        } catch (NoSuchFieldException e) {
            Log.d(TAG, null, e);
        }
        return null;
    }
    public static Object getField(String clsName, String fieldName) {
        if (TextUtils.isEmpty(clsName) || TextUtils.isEmpty(fieldName)) {
            return null;
        }
        Field field = null;
        try {
            try {
                field = Class.forName(clsName).getField(fieldName);
            } catch (ClassNotFoundException e) {
                Log.d(TAG, null, e);
            }
            return field;
        } catch (NoSuchFieldException e) {
            Log.d(TAG, null, e);
        }
        return null;
    }

    public static Object getStaticAttribute(String clsName, String fieldName) {
        if (TextUtils.isEmpty(clsName) || TextUtils.isEmpty(fieldName)) {
            return null;
        }
        Field field = null;
        try {
            Class clazz = null;
            try {
                clazz = Class.forName(clsName);
                field = Class.forName(clsName).getDeclaredField(fieldName);
            } catch (ClassNotFoundException e) {
                Log.d(TAG, null, e);
            }
            if (null != clazz && null != field) {
                return field.get(clazz);
            }

        } catch (NoSuchFieldException | IllegalAccessException e) {
            Log.d(TAG, null, e);
        }
        return null;
    }
    public static Object instance(String clazz)
        throws ClassNotFoundException, IllegalAccessException, InstantiationException {
        Class classType = Class.forName(clazz);
        Object object = classType.newInstance();
        return object;
    }

    public static Object instance(String clazz, Class[] paramTypes, Object[] params)
        throws ClassNotFoundException, NoSuchMethodException, IllegalAccessException,
               InvocationTargetException, InstantiationException {
        Class classType = Class.forName(clazz);
        Constructor constructor = classType.getConstructor(paramTypes);
        Object object = constructor.newInstance(params);
        return object;
    }

    public static Object instance(String clazz, Object[] params)
        throws ClassNotFoundException, NoSuchMethodException, InvocationTargetException,
               InstantiationException, IllegalAccessException {
        Class[] paramTypes = {Context.class};
        return instance(clazz, paramTypes, params);
    }
}
