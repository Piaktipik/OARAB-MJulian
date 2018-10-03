from time import strftime, time,localtime, sleep
import gps as gp
import threading

#

tiempoS = [2000, 0, 0, 0, 0, 0]
tiempoG = [2000, 0, 0, 0, 0, 0]
tiempo = [2000, 0, 0, 0, 0, 0]
stopT1 = False

def worker():
	"""funcion que realiza el trabajo en el thread"""
	#Listen on port 2947 (gpsd) of localhost
	session = gp.gps("localhost", "2947")
	session.stream(gp.WATCH_ENABLE | gp.WATCH_NEWSTYLE)

	while True:
		try:
			report = session.next()	# congela la ejecucion!
			# Wait for a 'TPV' report and display the current time
			# To see all report data, uncomment the line below
			# print report
			if report['class'] == 'TPV':
				if hasattr(report, 'time'):
					tiempoG[0] = int(report.time[:4])
					tiempoG[1] = int(report.time[5:7])
					tiempoG[2] = int(report.time[8:10])
					tiempoG[3] = int(report.time[11:13])
					tiempoG[4] = int(report.time[14:16])
					tiempoG[5] = int(report.time[17:19])
					#print(tiempoG)

			if stopT1:
				break

		except KeyError:
			pass
		except KeyboardInterrupt:
			quit()
		except StopIteration:
			session = None
			print "GPSD has terminated"
	return

threads = list()
t = threading.Thread(target=worker)
threads.append(t)
t.start()

while True:
	try:
		#print(strftime("%Y-%m-%dT%H:%M:%S", localtime()))
		tiempoS[0] = int(strftime("%Y", localtime()))
		tiempoS[1] = int(strftime("%m", localtime()))
		tiempoS[2] = int(strftime("%d", localtime()))
		tiempoS[3] = int(strftime("%H", localtime()))
		tiempoS[4] = int(strftime("%M", localtime()))
		tiempoS[5] = int(strftime("%S", localtime()))
		#print(localtime())
		#print (tiempoS)

		# tomamos el mayor de los tiempos
		for i in range(0,6):
			#print(i)
			if tiempoS[i] < tiempoG[i]:
				# tiempo sistema menor que gps, actualizamos
				tiempo = tiempoG
				break
			if tiempoS[i] > tiempoG[i]:
				# tiempo sistema mayor que gps, actualizamos
				tiempo = tiempoS
				break

		# si el tiempo Gps se congela usamos el tiempo el sistema para continuar
		'''if tiempoP == tiempo:
			print("iguales")
		else:
			print("diferentes")
			tiempoP = tiempo
		'''
		print(tiempo)


		sleep(1)
	except KeyboardInterrupt:
		stopT1 = True
		print("Saliendo")
		quit()