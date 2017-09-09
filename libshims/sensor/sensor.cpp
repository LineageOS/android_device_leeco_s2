
#include <stdint.h>
#include <sensor/Sensor.h>
#include <sensor/SensorManager.h>
#include <sensor/ISensorEventConnection.h>

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(__LP64__)
extern int _ZNK7android16SensorEventQueue12enableSensorEiili(
	void *queue, int32_t handle, int32_t samplingPeriodUs, 
	int64_t maxBatchReportLatencyUs, int reservedFlags);
#else
extern int _ZNK7android16SensorEventQueue12enableSensorEiixi(
	void *queue, int32_t handle, int32_t samplingPeriodUs, 
	int64_t maxBatchReportLatencyUs, int reservedFlags);
#endif

int _ZNK7android16SensorEventQueue12enableSensorEiiii(
	void *queue, int32_t handle, int32_t samplingPeriodUs, 
	int maxBatchReportLatencyUs, int reservedFlags)
{
#if defined(__LP64__)
	return _ZNK7android16SensorEventQueue12enableSensorEiili(
		queue, handle, samplingPeriodUs,
		maxBatchReportLatencyUs, reservedFlags);
#else
	return _ZNK7android16SensorEventQueue12enableSensorEiixi(
		queue, handle, samplingPeriodUs,
		maxBatchReportLatencyUs, reservedFlags);
#endif
}

#ifdef __cplusplus
}
#endif


namespace android {

static void dummy(void) {
	SensorManager& manager = SensorManager::getInstanceForPackage(String16(""));
	manager.getSensorList(NULL);
	Vector<Sensor> list;
	manager.getDynamicSensorList(list);
	manager.getDefaultSensor(0);
	manager.createEventQueue();
	manager.isDataInjectionEnabled();
	manager.createDirectChannel(0,	0, NULL);
	manager.destroyDirectChannel(0);
	manager.configureDirectChannel(0, 0, 0);
	Vector<float> floats;
	Vector<int32_t>	ints;
	manager.setOperationParameter(0, 0, floats, ints);

	sp<ISensorEventConnection> connection;
	SensorEventQueue* queue	= new SensorEventQueue(connection);
	delete queue;
	queue->getFd();
	sp<BitTube>	tube;
	queue->write(tube, NULL, 0);
	queue->read(NULL, 0);
	queue->waitForEvent();
	queue->wake();
	queue->enableSensor(NULL);
	queue->enableSensor(NULL, 0);
	queue->disableSensor((Sensor *)NULL);
	queue->setEventRate(NULL, 0);
	queue->enableSensor(0, 0, 0, 0);
	queue->disableSensor(0);
	queue->flush();
	queue->sendAck(NULL, 0);
	ASensorEvent event;
	queue->injectSensorEvent(event);

	Sensor*	sensor = new Sensor();
	sensor = new Sensor(NULL, 0);
	struct sensor_t	hwSensor;
	Sensor::uuid_t uuid;
	sensor = new Sensor(hwSensor, uuid);
	delete sensor;
	sensor->getName();
	sensor->getVendor();
	sensor->getHandle();
	sensor->getType();
	sensor->getMinValue();
	sensor->getMaxValue();
	sensor->getResolution();
	sensor->getPowerUsage();
	sensor->getMinDelay();
	sensor->getMinDelayNs();
	sensor->getVersion();
	sensor->getFifoReservedEventCount();
	sensor->getFifoMaxEventCount();
	sensor->getStringType();
	sensor->getRequiredPermission();
	sensor->isRequiredPermissionRuntime();
	sensor->getRequiredAppOp();
	sensor->getMaxDelay();
	sensor->getFlags();
	sensor->isWakeUpSensor();
	sensor->isDynamicSensor();
	sensor->hasAdditionalInfo();
	sensor->getHighestDirectReportRateLevel();
	sensor->isDirectChannelTypeSupported(0);
	sensor->getReportingMode();
	sensor->getUuid();
	sensor->getId();
	sensor->setId(0);
	sensor->getFlattenedSize();
	sensor->flatten(NULL, 0);
	sensor->unflatten(NULL,	0);
}

}; // namespace	android
