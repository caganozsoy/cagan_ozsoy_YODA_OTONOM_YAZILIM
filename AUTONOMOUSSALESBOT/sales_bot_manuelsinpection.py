from selenium import webdriver
from selenium.webdriver.common.by import By
from sentence_transformers import SentenceTransformer, util

# Preparing BERT model for our comparison
bert_model = SentenceTransformer('dbmdz/bert-base-turkish-cased')

# Since using safari for selenium we should adjust a webdriver for safari
driver = webdriver.Safari()

trendyol_link = "https://www.trendyol.com/hot-wheels/fast-furious-2021-toyota-gr-supra-hnw46-jbm00-p-983558788?boutiqueId=61&merchantId=110288.html"
product_title_trendyol = ".product-title"

akakce_link = "https://www.akakce.com/oyuncak-araba/en-ucuz-hot-wheels-fast-and-furious-premium-2021-toyota-gr-supra-hnw46-jbm00-fiyati,1222011215.html"
product_title_akakce = ".pdt_v8 h1"


def product_data_pull(url, name_css, price_css):
    driver.get(url)
    # since some sites are not refreshed fast enough while selenium is working, we have to adjust
    # our own waiting and refreshing time
    name = driver.find_element(By.CSS_SELECTOR, name_css).text
    price = driver.find_element(By.CSS_SELECTOR, price_css).text
    return name, price


try:
    t_name, t_price = product_data_pull(
        trendyol_link, product_title_trendyol, ".discounted")
    a_name, a_price = product_data_pull(
        akakce_link, product_title_akakce, ".pt_v8")

    # Using BERT for comparing names of the products using cosine similarity

    emb1 = bert_model.encode(t_name, convert_to_tensor=True)
    emb2 = bert_model.encode(a_name, convert_to_tensor=True)
    Score = util.cos_sim(emb1, emb2).item()

    print(f"Similarity Score: %{Score*100:.2f}")

    # If cosine similarity is greater than 0.85 which is multiplied with 100  after, then the name
    # of the products are same with some minor differences, this is the result of BERT.
    if Score > 0.85:
        print(
            f"Comparison is done, Price: Trendyol({t_price}) - Akakce({a_price})")

finally:
    driver.quit()


def clean_price(price_Tag):
    clean_priceTag = price_Tag.replace("TL", "").replace(
        ".", "").replace(",", ".").strip()
    return float(clean_priceTag)


t_number = clean_price(t_price)
a_number = clean_price(a_price)

if t_number < a_number:
    print(f"{a_name} with {a_number} is cheaper than {t_name} with {t_number}")

else:
    print(f"{a_name} with {a_number} is cheaper than {t_name} with {t_number}")
