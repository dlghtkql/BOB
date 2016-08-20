// gcc -o test_libusb libusb.c -lusb-1.0 -I/usr/include/libusb-1.0
// 2016.8.19

#include <stdio.h>
#include <libusb.h>

int printdev(libusb_device *dev){
	
	int i,j,k;
	struct libusb_device_descriptor desc;
	struct libusb_config_descriptor *config;
	const struct libusb_interface_descriptor *interdesc;
	const struct libusb_interface *interface;
	const struct libusb_endpoint_descriptor *epdesc;

	int r = libusb_get_device_descriptor(dev, &desc);
	if(r < 0){
		printf("Error : Device Descriptor getting failed.\n");
		return -1;
	}
	libusb_get_config_descriptor(dev, 0, &config);

	static const char * const typeattr[] = {
		"Control",
		"Isochronous",
		"Bulk",
		"Interrupt"
	};
	static const char * const syncattr[] = {
		"None",
		"Asynchronous",
		"Adaptive",
		"Synchronous"
	};
	static const char * const usage[] = {
		"Data",
		"Feedback",
		"Implicit feedback Data",
		"(reserved)"
	};

	printf("Device Descriptor:\n");
	printf("bLength: %u\n",desc.bLength);
	printf("bDescriptorType: %u\n",desc.bDescriptorType);
	printf("bcdUSB: %2x.%02x\n",desc.bcdUSB >> 8, desc.bcdUSB & 0xff);
	printf("bDeviceClass: %u\n",desc.bDeviceClass);
	printf("bDeviceSubClass: %u\n",desc.bDeviceSubClass);
	printf("bDeviceProtocol: %u\n",desc.bDeviceProtocol);
	printf("bMaxPacketSize0: %u\n",desc.bMaxPacketSize0);
	printf("ID Vendor : %4.0x\n", (int) desc.idVendor);
	printf("ID Product : %4.0x\n", (int) desc.idProduct);
	printf("bcdDevice: %2x.%02x\n",desc.bcdDevice >> 8 , desc.bcdDevice & 0xff);
	printf("iManufacturer: %u\n",desc.iManufacturer);
	printf("iProduct: %u\n",desc.iProduct);
	printf("iSerial: %u\n",desc.iSerialNumber);
	printf("bNumConfigurations: %u\n",desc.bNumConfigurations);
	printf("\n");

	printf("Configuration Descriptor:\n");
	printf("bLength: %u\n",config->bLength);
	printf("bDescriptorType: %u\n",config->bDescriptorType);
	printf("wTotalLength: %u\n",config->wTotalLength);
	printf("bNumInterfaces: %u\n",config->bNumInterfaces);
	printf("bConfigurationValue: %u\n",config->bConfigurationValue);
	printf("iConfiguration: %u\n",config->iConfiguration);
	printf("bmAttributes: %0x\n",config->bmAttributes);
	if (!(config->bmAttributes & 0x80))
		printf("      (Missing must-be-set bit!)\n");
	if (config->bmAttributes & 0x40)
		printf("      Self Powered\n");
	else
		printf("      (Bus Powered)\n");
	if (config->bmAttributes & 0x20)
		printf("      Remote Wakeup\n");
	if (config->bmAttributes & 0x10)
		printf("      Battery Powered\n");
	printf("MaxPower: %dmA\n",config->MaxPower);
	printf("\n");


	for(i = 0 ;i<(int)config->bNumInterfaces ; i++)
	{
		interface = &config->interface[i];
		for(j=0 ; j<interface->num_altsetting; j++)
		{
			interdesc = &interface->altsetting[j];
			printf("Interface Descriptor:\n");
			printf("bLength: %u\n",interdesc->bLength);
			printf("bDescriptorType %u\n",interdesc->bDescriptorType);
			printf("bInterfaceNumber: %u\n",interdesc->bInterfaceNumber);
			printf("bAlternateSetting: %u\n",interdesc->bAlternateSetting);
			printf("bNumEndpoints: %u\n",interdesc->bNumEndpoints);
			printf("bInterfaceClass: %u\n",interdesc->bInterfaceClass);
			printf("bInterfaceSubClass: %u\n",interdesc->bInterfaceSubClass);
			printf("bInterfaceProtocol: %u\n",interdesc->bInterfaceProtocol);
			printf("iInterface: %u\n",interdesc->iInterface);
		}

			for(k=0 ; k<(int)interdesc->bNumEndpoints; k++)
			{
				epdesc = &interdesc->endpoint[k];
				printf("\n");
				printf("Endpoint Descriptor:\n");
				printf("bLength: %u\n",epdesc->bLength);
				printf("bDescriptorType: %u\n",epdesc->bDescriptorType);
				printf("bEndpointAddress: %x\n",epdesc->bEndpointAddress);
				printf("bmAttributes: %u\n",epdesc->bmAttributes);
				printf("  Transfer Type: %s\n", typeattr[epdesc->bmAttributes & 3]);
				printf("  Synch Type: %s\n",syncattr[(epdesc->bmAttributes >> 2) & 3]);
				printf("  Usage Type: %s\n",usage[(epdesc->bmAttributes >> 4) & 3]);
				printf("wMaxPacketSize: %u\n",epdesc->wMaxPacketSize);
				printf("bInterval: %u\n",epdesc->bInterval);
			}
	}
	printf("==================================\n");	
}

int main(int argc, char *argv[]){
	libusb_device **devs;
	libusb_context *ctx = NULL;
	int r;
	ssize_t cnt;
	if((r = libusb_init(&ctx)) < 0){
		printf("Error : %d\n", r);
		return -1;
	}
	libusb_set_debug(ctx, 3); // set debug level

	if((cnt = libusb_get_device_list(ctx, &devs)) < 0){
		printf("Error : Getting Device list failed.\n");
		return -1;
	}

	for(int i = 0; i < cnt; i++){
		printdev(devs[i]);
	}

	libusb_free_device_list(devs, 1);
	libusb_exit(ctx);

	return 0;
}
