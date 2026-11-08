# Ödev 2
# Git ve GitHub Temel Kavramları
1. Repo: Yazılım dünyasında kod dosyaları olsun, tüm verilerin tutulabildiği dijital bir depolama sistemi. Aslında bilgisayarımızdaki dosyalar birer rapordur.
2. Branch: Reponun içindeki dallanmalardır. Bu dallar birbirinin kopyası olarak üretilir ancak içinde yapılan değişiklik sadece ona özgündür diğer branchleri asla etkilemez.
3. Commit: Bir branchin içinde yapılan değişikliği git ile onaylayan operasyondur. Commitlenmemiş hiçbir değişiklik kaydedilmez ve uygulanmaz. Örneğin VScode'a bağlanmış GitHub hesabınızda kodda yapılan değişiklik commit edilmez ise GitHub reposunda gözükmez.
4. Merge: Adı üstünde birleştirme operasyonu iki farklı branch veya dosyayı birleştirmek için kullanılır. Örneğin bu ödevde odev_2 branchi ile main branchi merge edilip bir branch haline getirilecektir.
5. Pull: Başka bir repodan alınan değişiklikleri kendi branchimizde kullanmak veya güncellemek amaçlı yapılan bir operasyondur. Fetch ve Merge operasyonlarının birleşimidir.
6. Push: Push operatörü ise repoya aktarılmak istenen değişiklikleri aktarır. Ve tabii ki de bunun için commit yapılması gerekir. Bunun için git klasörü içinde kendi ornkodlar dosyamı önce pushladım sonra ise kendime pulladım. Bu yüzden 2. bir dosya örneğini size koydum. 

# Takımlar Neden Git ve GitHub Kullanır?
# 1. Versiyon Kontrolüne Neden İhtiyaç Duyulur?
Versiyon kontrolü, bir projede yapılan değişikliklerin kayıt altına alınmasını ve önceki sürümlere geri dönülebilmesini sağlar. Versiyon kontrolü kullanılmazsa hangi dosyanın en güncel olduğu karışabilir, çalışan kodun üzerine hatalı kod yazılabilir ve ekip üyelerinin yaptığı değişiklikler kaybolabilir.Git'teki commit sistemi sayesinde projenin belirli anlardaki durumları kaydedilir. Her commit'in bir kimliği,çıklaması ve tarihi bulunur.
# 2. Git ile GitHub arasındaki fark:
Git, yazılım projelerindeki değişiklikleri takip etmeye yarayan dağıtık bir versiyon kontrol sistemidir. İnternet bağlantısı olmadan da bilgisayarımızda commit oluşturabilir ve geçmişi inceleyebiliriz. GitHub ise Git depolarını internet üzerinde saklamamızı, paylaşmamızı ve başkalarıyla birlikte geliştirmemizi sağlayan bir platformdur. Temel fark, Git'in versiyon kontrolünü gerçekleştiren araç olması, GitHub'ın ise bu araçla oluşturulan projeleri barındırması ve ekip çalışmasını kolaylaştırmasıdır. GitHub'a alternatif olarak GitLab kullanılabilir.
# 3. Ekip çalışmasına katkıları:
Git ve GitHub, birden fazla kişinin aynı proje üzerinde birbirinin çalışmasını bozmadan geliştirme yapmasını sağlar. Bunun için ekip üyeleri farklı branch'ler oluşturarak bağımsız çalışabilir. Örneğin otonom araç projesinde bir kişi görüntü işleme algoritmasını geliştirirken başka bir kişi motor kontrol algoritmasını geliştirebilir. İki kişi farklı branch'lerde çalıştığından değişikliklerini ayrı ayrı hazırlayabilir.
- Pull Request: Bir branch'te yapılan değişikliklerin başka bir branch'e eklenmesi için oluşturulan inceleme ve birleştirme talebidir.
- Code Review: Yazılan kodun diğer ekip üyeleri tarafından incelenmesidir. Hataların erkenden fark edilmesini sağlar.
- Merge Conflict: İki farklı branch'te aynı kod bölümü uyumsuz şekilde değiştirildiğinde Git'in hangi değişikliği kullanacağına otomatik karar verememesidir.
Örnek: İki kişi aynı dosyadaki aracın maksimum hız değerini farklı şekilde değiştirmişse merge conflict oluşabilir. Ekip üyeleri kodu inceleyerek doğru değeri belirler, çakışmayı çözer ve branch'leri birleştirir.
# 4. GitHub’ın proje yönetimi özellikleri:
GitHub yalnızca kod saklamak için değil, projelerin planlanması ve yönetilmesi için de kullanılabilir.
- GitHub Issues: Projede yapılması gereken görevlerin, hataların ve geliştirme önerilerinin kaydedilmesini sağlar.
- GitHub Projects: Görevlerin planlanmasını ve ilerlemelerinin takip edilmesini sağlar. Görevler To Do, In Progress, Done gibi aşamalara ayrılabilir.
- README.md: Projenin amacını, kurulum aşamalarını, kullanılan teknolojileri ve çalışma şeklini açıklamak için kullanılır.
- GitHub Wiki: Projeyle ilgili daha kapsamlı teknik dokümantasyonun tutulmasını sağlar.
- GitHub Actions: Belirli olaylar gerçekleştiğinde test, derleme veya kontrol işlemlerinin otomatik olarak yapılmasını sağlar.
Örnek: Bir otonom araç takımında LiDAR entegrasyonu GitHub Issues üzerinden bir üyeye atanabilir. Üye görevini tamamladığında pull request oluşturur. GitHub Actions kodun testlerini çalıştırabilir ve inceleme tamamlandıktan sonra değişiklik projeye eklenebilir.
# 5. Kulüp ve takımlar için önemi:
Öğrenci takımlarında üyeler zamanla mezun olmakta ve yerlerine yeni öğrenciler katılmaktadır. Git ve GitHub, geliştirilen yazılımın ve teknik bilginin kaybolmaması açısından oldukça önemlidir. Takım üyeleri yaptıkları çalışmaları düzenli olarak commit'leyebilir ve GitHub üzerinde saklayabilir. Ayrıca README dosyaları, teknik dokümanlar ve açıklamalı kodlar sayesinde projenin nasıl çalıştığı kayıt altına alınabilir. Örnek: Bir otonom araç takımının önceki üyeleri ROS2 ile navigasyon sistemi geliştirmiş olsun. Bu üyeler mezun olduğunda yeni katılan öğrenciler GitHub üzerinden mevcut kodları inceleyebilir, kurulum talimatlarını okuyabilir ve projeyi yeniden geliştirmek zorunda kalmadan kaldığı yerden devam edebilir. Böylece hem zaman kaybı önlenir hem de takımın yıllar içerisinde kazandığı teknik bilgi korunur.
# 6. Otonom araç projesindeki yeri:
Otonom araç projelerinde görüntü işleme, sensör sistemleri, navigasyon, kontrol ve yapay zekâ gibi farklı alanlarda çalışan ekipler bulunur. Git sayesinde bu ekipler kendi branch'lerinde paralel çalışabilir. Örneğin görüntü işleme ekibi kameradan nesne tespiti yapan bir algoritma geliştirirken kontrol ekibi aracın direksiyon ve hız kontrolünü geliştirebilir. Geliştirilen yazılımlar test edildikten sonra pull request ve code review süreçlerinden geçirilerek ana branch'e eklenebilir. Saha testlerinde kullanılan sürümleri takip etmek için Git tag sistemi kullanılabilir. Örneğin v1.0-test etiketiyle test edilen yazılım sürümü kaydedilebilir. Bir sonraki testte v1.1-test sürümü kullanılarak iki sürümün performansı karşılaştırılabilir. Bu sayede bir hata oluştuğunda hangi yazılım sürümünün kullanıldığı belirlenebilir ve önceki çalışan sürüme dönülebilir. Bazı kulüpler veya teknofest takımları GitHub üzerinden repolarını public olarak açıyor ancak neredeyse hepsinin reposu private olduğundan erişilemiyor.
# 7. Dikkat edilmesi gerekenler:
Git ve GitHub yanlış kullanıldığında bazı sorunlara neden olabilir.
- Yanlış branch'e commit atmak: Geliştirme aşamasındaki kodların yanlışlıkla ana branch'e eklenmesine neden olabilir. Bunu önlemek için ayrı branch'lerde çalışılmalı ve pull request kullanılmalıdır. 
- Gereksiz dosyaların eklenmesi: git add . komutu kullanıldığında projedeki istenmeyen dosyalar da Git'e eklenebilir. Bunun yerine dosyalar seçilerek eklenmeli ve .gitignore kullanılmalıdır. Örnek vermek gerekirse git klasöründeki örnek dosyayı pushlayacağım diye yanlışlıkla .add diyerek tüm dosyaları pushladım ancak commit etmediğim için geri çevirebildim. Sonrasında ise yedekleme yaparak bunun ötesinde geçtim ve yanlış branchlemeyi de önledim. 
- Merge conflict: Aynı kod üzerinde yapılan farklı değişiklikler birleştirme sırasında çakışabilir. Çakışmalar ekip üyeleri tarafından incelenerek çözülmelidir.
- Hatalı kodların paylaşılması: Test edilmemiş kodlar projenin çalışmasını bozabilir. Bu nedenle değişiklikler ana branch'e eklenmeden önce test edilmeli ve code review yapılmalıdır.
- Gizli bilgilerin paylaşılması: API anahtarları, şifreler veya özel dosyalar yanlışlıkla herkese açık GitHub depolarına yüklenebilir. Gizli bilgiler Git'e eklenmemeli, yanlışlıkla paylaşılmışlarsa ilgili anahtarlar iptal edilmelidir.
Sonuç olarak, Git ve GitHub'ın düzenli kullanılması yazılım geliştirme sürecini daha güvenli, takip edilebilir ve ekip çalışmasına uygun hale getirir.

    
