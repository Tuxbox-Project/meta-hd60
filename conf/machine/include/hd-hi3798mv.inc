# We know about rtc
MACHINE_FEATURES_BACKFILL_CONSIDERED = "rtc"

PREFERRED_VERSION_linux-hd = "4.4.35"

#Provider
PREFERRED_PROVIDER_virtual/kernel ?= "linux-hd"
PREFERRED_PROVIDER_virtual/egl ?= "hd-mali-${MACHINE}"
PREFERRED_PROVIDER_virtual/libgles1 ?= "hd-mali-${MACHINE}"
PREFERRED_PROVIDER_virtual/libgles2 ?= "hd-mali-${MACHINE}"
PREFERRED_PROVIDER_virtual/libopencl ?= "hd-mali-${MACHINE}"
PREFERRED_PROVIDER_virtual/blindscan-dvbs ?= "hd-dvb-modules-${MACHINE}"
PREFERRED_PROVIDER_virtual/blindscan-dvbc ?= "hd-dvb-modules-${MACHINE}"

#require conf/machine/include/arm/arch-armv8.inc
require conf/machine/include/tune-cortexa15.inc
require conf/machine/include/hd-essential.inc

# Kodi
EXTRA_OECONF_append_pn-kodi += " --with-gpu=mali"

MACHINE_EXTRA_RRECOMMENDS = " \
	ffmpeg \
	ntfs-3g \
"

MACHINE_FEATURES += " alsa usbhost wlan"

IMAGEVERSION := "${DISTRO_NAME}-${DISTRO_VERSION}-${DATE}"
IMAGEVERSION[vardepsexclude] = "DATE"
IMAGEDIR ?= "hd"
IMAGEDIRRECOVERY ?= "${MACHINE}-recovery"

image_packaging() {
	mkdir -p ${IMGDEPLOYDIR}/${IMAGEDIR}/${MACHINE}; \
	mkdir -p ${IMGDEPLOYDIR}/${IMAGEDIRRECOVERY}/${MACHINE}; \
	cp ${DEPLOY_DIR_IMAGE}/uImage ${IMGDEPLOYDIR}/${IMAGEDIR}/${MACHINE}/uImage; \
	cp ${IMGDEPLOYDIR}/${IMAGE_NAME}${IMAGE_NAME_SUFFIX}.tar.bz2 ${IMGDEPLOYDIR}/${IMAGEDIR}/${MACHINE}/rootfs.tar.bz2; \
	echo "${IMAGEVERSION}" > ${IMGDEPLOYDIR}/${IMAGEDIR}/${MACHINE}/imageversion; \
	echo "${IMAGEVERSION}_emmc.zip" > ${IMGDEPLOYDIR}/${IMAGEDIR}/unforce_${MACHINE}.txt; \
	echo "Rename the unforce_${MACHINE}.txt to force_${MACHINE}.txt and move it to the root of your usb-stick" > ${IMGDEPLOYDIR}/${IMAGEDIR}/${MACHINE}/force_${MACHINE}_READ.ME; \
	echo "When you enter the recovery menu then it will force to install the image ${IMAGEVERSION}_emmc.zip in the image-slot1" >> ${IMGDEPLOYDIR}/${IMAGEDIR}/${MACHINE}/force_${MACHINE}_READ.ME; \
	cp ${IMGDEPLOYDIR}/bootoptions.img ${IMGDEPLOYDIR}/${IMAGEDIRRECOVERY}/${MACHINE}/bootoptions.img; \
	cp ${DEPLOY_DIR_IMAGE}/hd-partitions-${MACHINE}/fastboot.bin ${IMGDEPLOYDIR}/${IMAGEDIRRECOVERY}/fastboot.bin; \
	cp ${DEPLOY_DIR_IMAGE}/hd-partitions-${MACHINE}/bootargs-8gb.bin ${IMGDEPLOYDIR}/${IMAGEDIRRECOVERY}/bootargs.bin; \
	cp ${DEPLOY_DIR_IMAGE}/hd-partitions-${MACHINE}/${MACHINE}/fastboot.bin ${IMGDEPLOYDIR}/${IMAGEDIRRECOVERY}/${MACHINE}/fastboot.bin; \
	cp ${DEPLOY_DIR_IMAGE}/hd-partitions-${MACHINE}/${MACHINE}/bootargs-8gb.bin ${IMGDEPLOYDIR}/${IMAGEDIRRECOVERY}/${MACHINE}/bootargs.bin; \
	cp ${DEPLOY_DIR_IMAGE}/hd-partitions-${MACHINE}/${MACHINE}/pq_param.bin ${IMGDEPLOYDIR}/${IMAGEDIRRECOVERY}/${MACHINE}/pq_param.bin; \
	cp ${DEPLOY_DIR_IMAGE}/hd-partitions-${MACHINE}/${MACHINE}/baseparam.img ${IMGDEPLOYDIR}/${IMAGEDIRRECOVERY}/${MACHINE}/baseparam.img; \
	if [ -e ${DEPLOY_DIR_IMAGE}/hd-partitions-${MACHINE}/${MACHINE}/logo.img ]; then \
		cp ${DEPLOY_DIR_IMAGE}/hd-partitions-${MACHINE}/${MACHINE}/logo.img ${IMGDEPLOYDIR}/${IMAGEDIRRECOVERY}/${MACHINE}/logo.img; \
	fi
	if [ -e ${DEPLOY_DIR_IMAGE}/hd-partitions-${MACHINE}/${MACHINE}/recovery.bin ]; then \
		cp ${DEPLOY_DIR_IMAGE}/hd-partitions-${MACHINE}/${MACHINE}/recovery.bin ${IMGDEPLOYDIR}/${IMAGEDIRRECOVERY}/${MACHINE}/recovery.bin; \
	fi
	echo "${MACHINEBUILD}-${DATE}-RECOVERY" > ${IMGDEPLOYDIR}/${IMAGEDIRRECOVERY}/${MACHINE}/recoveryversion; \
	echo "***** ACHTUNG *****" >${IMGDEPLOYDIR}/${IMAGEDIRRECOVERY}/recovery_${MACHINEBUILD}_lies.mich; \
	echo "Das RECOVERY wird nur benöetigt wenn es Probleme gibt beim zugriff auf das MULTIBOOT MENÜ" >>${IMGDEPLOYDIR}/${IMAGEDIRRECOVERY}/recovery_${MACHINEBUILD}_lies.mich; \
	echo "Das ${IMAGE_NAME}_recovery_usb.zip sollte normalerweise einmal installiert werden (oder wenn es ein Update gibt.)" >>${IMGDEPLOYDIR}/${IMAGEDIRRECOVERY}/recovery_${MACHINEBUILD}_lies.mich; \
	echo "Dies ist erforderlich, um Probleme mit dem Images zuvermeiden,wenn sich der Aufbau der Partition ändert (bootargs)." >>${IMGDEPLOYDIR}/${IMAGEDIRRECOVERY}/recovery_${MACHINEBUILD}_lies.mich; \
	echo "Die Änderungen können alle Daten im Flash Löschen, nur Installieren wenn es notwendig ist." >>${IMGDEPLOYDIR}/${IMAGEDIRRECOVERY}/recovery_${MACHINEBUILD}_lies.mich; \
	echo "***** ATTENTION *****" >${IMGDEPLOYDIR}/${IMAGEDIRRECOVERY}/recovery_${MACHINEBUILD}_READ.ME; \
	echo "This RECOVERY only need when you have issue access the MULTIBOOT MENU" >>${IMGDEPLOYDIR}/${IMAGEDIRRECOVERY}/recovery_${MACHINEBUILD}_READ.ME; \
	echo "The ${IMAGE_NAME}_recovery_usb.zip should been install normally once (or if there is an update.)" >>${IMGDEPLOYDIR}/${IMAGEDIRRECOVERY}/recovery_${MACHINEBUILD}_READ.ME; \
	echo "This is needed for avoid images conflicts as the partition layout (bootargs) is of huge essential." >>${IMGDEPLOYDIR}/${IMAGEDIRRECOVERY}/recovery_${MACHINEBUILD}_READ.ME; \
	echo "A small change can destroy all your install images. So we can better leave it and not install it if it's not need." >>${IMGDEPLOYDIR}/${IMAGEDIRRECOVERY}/recovery_${MACHINEBUILD}_READ.ME; \
	cd ${IMGDEPLOYDIR}/${IMAGEDIRRECOVERY}; \
	zip -r ../${DISTRO_NAME}-${DISTRO_VERSION}-${MACHINE}_recovery_emmc.zip *; \
	cd ..; \
	rm -Rf ${IMAGEDIRRECOVERY}; \
	cd ${IMGDEPLOYDIR}/${IMAGEDIR}; \
	zip -r ../${DISTRO_NAME}-${DISTRO_VERSION}-${MACHINE}_mmc.zip unforce_${MACHINE}.txt ${MACHINE}/force_${MACHINE}_READ.ME ${MACHINE}/imageversion ${MACHINE}/uImage ${MACHINE}/rootfs.tar.bz2; \
	cd ..; \
	rm -Rf ${IMAGEDIR}; \
}

IMAGE_POSTPROCESS_COMMAND += "image_packaging; "
